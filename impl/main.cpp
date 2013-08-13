#include <stdlib.h>
#include <stdio.h>
#include "IBtrfParser.h"
#include "IBtrfRootBlock.h"
#include "IBtrfBlock.h"
#include "ITmlFile.h"
#include <windows.h>
#include <assert.h>

#include <math.h>

#include <list>
#include <deque>
#include <string>
#include <vector>

void inverse4x4(const float a[][4], float b[][4])
{
    float s0 = a[0][0] * a[1][1] - a[1][0] * a[0][1];
    float s1 = a[0][0] * a[1][2] - a[1][0] * a[0][2];
    float s2 = a[0][0] * a[1][3] - a[1][0] * a[0][3];
    float s3 = a[0][1] * a[1][2] - a[1][1] * a[0][2];
    float s4 = a[0][1] * a[1][3] - a[1][1] * a[0][3];
    float s5 = a[0][2] * a[1][3] - a[1][2] * a[0][3];

    float c5 = a[2][2] * a[3][3] - a[3][2] * a[2][3];
    float c4 = a[2][1] * a[3][3] - a[3][1] * a[2][3];
    float c3 = a[2][1] * a[3][2] - a[3][1] * a[2][2];
    float c2 = a[2][0] * a[3][3] - a[3][0] * a[2][3];
    float c1 = a[2][0] * a[3][2] - a[3][0] * a[2][2];
    float c0 = a[2][0] * a[3][1] - a[3][0] * a[2][1];

    // Should check for 0 determinant
    float invdet = 1.0 / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);

    b[0][0] = ( a[1][1] * c5 - a[1][2] * c4 + a[1][3] * c3) * invdet;
    b[0][1] = (-a[0][1] * c5 + a[0][2] * c4 - a[0][3] * c3) * invdet;
    b[0][2] = ( a[3][1] * s5 - a[3][2] * s4 + a[3][3] * s3) * invdet;
    b[0][3] = (-a[2][1] * s5 + a[2][2] * s4 - a[2][3] * s3) * invdet;

    b[1][0] = (-a[1][0] * c5 + a[1][2] * c2 - a[1][3] * c1) * invdet;
    b[1][1] = ( a[0][0] * c5 - a[0][2] * c2 + a[0][3] * c1) * invdet;
    b[1][2] = (-a[3][0] * s5 + a[3][2] * s2 - a[3][3] * s1) * invdet;
    b[1][3] = ( a[2][0] * s5 - a[2][2] * s2 + a[2][3] * s1) * invdet;

    b[2][0] = ( a[1][0] * c4 - a[1][1] * c2 + a[1][3] * c0) * invdet;
    b[2][1] = (-a[0][0] * c4 + a[0][1] * c2 - a[0][3] * c0) * invdet;
    b[2][2] = ( a[3][0] * s4 - a[3][1] * s2 + a[3][3] * s0) * invdet;
    b[2][3] = (-a[2][0] * s4 + a[2][1] * s2 - a[2][3] * s0) * invdet;

    b[3][0] = (-a[1][0] * c3 + a[1][1] * c1 - a[1][2] * c0) * invdet;
    b[3][1] = ( a[0][0] * c3 - a[0][1] * c1 + a[0][2] * c0) * invdet;
    b[3][2] = (-a[3][0] * s3 + a[3][1] * s1 - a[3][2] * s0) * invdet;
    b[3][3] = ( a[2][0] * s3 - a[2][1] * s1 + a[2][2] * s0) * invdet;
}

struct Material {
	const char* mtl_name;
	const char* texture_name;

	int mtl_id;

	float power;
	float self_illumi;

	int ambient;
	int diffuse;
	int specular;
};

struct Vector3D {
	float x;
	float y;
	float z;
};

struct Vector2D {
	float x;
	float y;
};

struct Bone {
	struct VertexInfluence {
		float vertexIndex;
		float weight;
	};
	const char* name;
	int verticeCount;
	VertexInfluence *boneVertex;
	Vector3D *vertexOffset;
};

struct VertexBoneAssociation {
	float weight;
	int boneIndex;
};

struct Mesh {
	int textureId;

	int vertexCount;
	Vector3D *vertex_array;
	int normalCount;
	Vector3D *normal_array;
	int texelCount;
	Vector2D *texel_array;
	int indexCount;
	short *index_array;

	std::vector<Bone> boneVertices;
	std::vector<std::list<VertexBoneAssociation>> boneVerticesAssociation;
	float *transformMatrix;	//[16]
};

void gdb_dump_vertex_bone_assoc(std::vector<std::list<VertexBoneAssociation>> boneVerticesAssociation, int i) {
	std::list<VertexBoneAssociation>::const_iterator it, itEnd;

	for(it = boneVerticesAssociation[i].cbegin(), itEnd = boneVerticesAssociation[i].cend(); it != itEnd; ++it)
		printf("boneVertex[%d] = { idx = %d, weight = %f}\n", i, it->boneIndex, it->weight);
}

struct Geometry {
	const char* mesh_name;

	int material_id;
	int channel_id;

	std::list<Mesh> meshes;
};

struct BoneTransform {
	const char* name;
	float *transformMatrix; //[16]
};

struct BoneAnimation {
	const char* boneName;

	int pos_time_count;
	int *pos_time_array;
	int pos_key_count;
	float *pos_key_array;

	int rot_time_count;
	int *rot_time_array;
	int rot_key_count;
	float *rot_key_array;
};

struct AnimationChannel {
	std::list<BoneAnimation> boneAnims;
};

struct Model {
	std::list<Material> materials;
	std::list<Geometry> geometries;
	std::vector<BoneTransform> boneTransformMatrix;
	std::list<AnimationChannel> animations;
};

void writeObj(const char* texturePath, int vertexArraySize, float *vertexArray, int texCoordArraySize, float *texCoordArray, int normalsArraySize, float *normalsArray, int indicesArraySize, unsigned short *indicesArray) {
	printf("#Vertices\n");
	for(int i=0; i < vertexArraySize; i+=3)
		printf("v %f %f %f\n", vertexArray[i], vertexArray[i+1], vertexArray[i+2]);

	printf("#Normals\n");
	for(int i=0; i < normalsArraySize; i+=3)
		printf("vn %f %f %f\n", normalsArray[i], normalsArray[i+1], normalsArray[i+2]);

	printf("#Tex coord\n");
	for(int i=0; i < texCoordArraySize; i+=2)
		printf("vt %f %f\n", texCoordArray[i], texCoordArray[i+1]);

	printf("#Faces\n");
	for(int i=0; i < indicesArraySize; i+=3)
		printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
				indicesArray[i]+1, indicesArray[i]+1, indicesArray[i]+1,
				indicesArray[i+1]+1, indicesArray[i+1]+1, indicesArray[i+1]+1,
				indicesArray[i+2]+1, indicesArray[i+2]+1, indicesArray[i+2]+1);
}

void writeCollada(const Model& model, FILE* file) {
	std::list<Material>::const_iterator materialIt, materialEnd;
	std::list<Geometry>::const_iterator geometryIt, geometryEnd;
	std::list<Mesh>::const_iterator meshIt, meshEnd;
	std::list<Bone>::const_iterator boneIt, boneEnd;
	std::list<AnimationChannel>::const_iterator animChannelIt, animChannelEnd;
	std::list<BoneAnimation>::const_iterator boneAnimIt, boneAnimEnd;
	int index;

	float scaleY = 1;

	fprintf(file,
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">\n"
		"  <asset>\n"
		"    <created>2013-07-09T20:33:25</created>\n"
		"    <modified>2013-07-09T20:33:25</modified>\n"
		"    <unit name=\"meter\" meter=\"1\"/>\n"
		"    <up_axis>Z_UP</up_axis>\n"
		"  </asset>\n");

	fprintf(file, "  <library_images>\n");
	for(index = 0, materialIt = model.materials.cbegin(), materialEnd = model.materials.cend(); materialIt != materialEnd; ++materialIt) {
		const Material& material = *materialIt;
		const char *textureFileNameStart;

		if(material.texture_name == 0)
			textureFileNameStart = "no_file";
		else {
			textureFileNameStart = material.texture_name + strlen(material.texture_name) - 1;
			while(textureFileNameStart > material.texture_name && *textureFileNameStart != '/' && *textureFileNameStart != '\\')
				textureFileNameStart--;

			if(*textureFileNameStart == '/' || *textureFileNameStart == '\\')
				textureFileNameStart++;
		}

		fprintf(file,
			"    <image id=\"tex-%d\">\n"
			"      <init_from>%s</init_from>\n"
			"    </image>\n", index, textureFileNameStart);

		index++;
	}
	fprintf(file, "  </library_images>\n");

	fprintf(file, "  <library_effects>\n");
	for(index = 0, materialIt = model.materials.cbegin(), materialEnd = model.materials.cend(); materialIt != materialEnd; ++materialIt) {
		//const Material& material = *materialIt;

		fprintf(file,
			"    <effect id=\"fx-%d\">\n"
			"      <profile_COMMON>\n"
			"        <newparam sid=\"surface\">\n"
			"          <surface type=\"2D\">\n"
			"            <init_from>tex-%d</init_from>\n"
			"          </surface>\n"
			"        </newparam>\n"
			"        <newparam sid=\"sampler\">\n"
			"          <sampler2D>\n"
			"            <source>surface</source>\n"
			"          </sampler2D>\n"
			"        </newparam>\n"
			"        <technique sid=\"common\">\n"
			"          <phong>\n"
			"            <emission>\n"
			"              <color sid=\"emission\">0 0 0 1</color>\n"
			"            </emission>\n"
			"            <ambient>\n"
			"              <texture texture=\"sampler\" texcoord=\"UVMap\"/>\n"
			"            </ambient>\n"
			"            <diffuse>\n"
			"              <texture texture=\"sampler\" texcoord=\"UVMap\"/>\n"
			"            </diffuse>\n"
			"            <specular>\n"
			"              <texture texture=\"sampler\" texcoord=\"UVMap\"/>\n"
			"            </specular>\n"
			"            <shininess>\n"
			"              <float sid=\"shininess\">5</float>\n"
			"            </shininess>\n"
			"            <index_of_refraction>\n"
			"              <float sid=\"index_of_refraction\">1</float>\n"
			"            </index_of_refraction>\n"
			"          </phong>\n"
			"        </technique>\n"
			"        <extra>\n"
			"          <technique profile=\"GOOGLEEARTH\">\n"
			"            <double_sided>1</double_sided>\n"
			"          </technique>\n"
			"        </extra>\n"
			"      </profile_COMMON>\n"
			"      <extra>\n"
			"        <technique profile=\"MAX3D\">\n"
			"          <double_sided>1</double_sided>\n"
			"        </technique>\n"
			"      </extra>\n"
			"    </effect>\n", index, index);

		index++;
	}
	fprintf(file, "  </library_effects>\n");

	fprintf(file, "  <library_materials>\n");
	for(index = 0, materialIt = model.materials.cbegin(), materialEnd = model.materials.cend(); materialIt != materialEnd; ++materialIt) {
		fprintf(file,
			"    <material id=\"mat-%d\" name=\"material_%d\">\n"
			"      <instance_effect url=\"#fx-%d\"/>\n"
			"    </material>\n", index, index, index);

		index++;
	}
	fprintf(file, "  </library_materials>\n");

	fprintf(file, "  <library_geometries>\n");
	for(geometryIt = model.geometries.cbegin(), geometryEnd = model.geometries.cend(); geometryIt != geometryEnd; ++geometryIt) {
		const Geometry& geometry = *geometryIt;

		for(index = 0, meshIt = geometry.meshes.cbegin(), meshEnd = geometry.meshes.cend(); meshIt != meshEnd; ++meshIt) {
			const Mesh& mesh = *meshIt;

			fprintf(file, "    <geometry id=\"geom-%s-%d\" name=\"%s-%d\">\n", geometry.mesh_name, index, geometry.mesh_name, index);

			//Vertices positions
			fprintf(file,
				"      <mesh>\n"
				"        <source id=\"pos-%s-%d\">\n"
				"          <float_array id=\"posa-%s-%d\" count=\"%d\"> ", geometry.mesh_name, index, geometry.mesh_name, index, mesh.vertexCount*3);

			for(int i=0; i < mesh.vertexCount; i++)
				fprintf(file, "%f %f %f ", mesh.vertex_array[i].x, mesh.vertex_array[i].y*scaleY, mesh.vertex_array[i].z);

			fprintf(file, "</float_array>\n"
				"          <technique_common>\n"
				"            <accessor source=\"#posa-%s-%d\" count=\"%d\" stride=\"3\">\n"
				"              <param name=\"X\" type=\"float\"/>\n"
				"              <param name=\"Y\" type=\"float\"/>\n"
				"              <param name=\"Z\" type=\"float\"/>\n"
				"            </accessor>\n"
				"          </technique_common>\n"
				"        </source>\n", geometry.mesh_name, index, mesh.vertexCount);

			//Normals
			fprintf(file,
				"        <source id=\"norm-%s-%d\">\n"
				"          <float_array id=\"norma-%s-%d\" count=\"%d\"> ", geometry.mesh_name, index, geometry.mesh_name, index, mesh.normalCount*3);

			for(int i=0; i < mesh.normalCount; i++)
				fprintf(file, "%f %f %f ", mesh.normal_array[i].x, mesh.normal_array[i].y*scaleY, mesh.normal_array[i].z);

			fprintf(file, "</float_array>\n"
				"          <technique_common>\n"
				"            <accessor source=\"#norma-%s-%d\" count=\"%d\" stride=\"3\">\n"
				"              <param name=\"X\" type=\"float\"/>\n"
				"              <param name=\"Y\" type=\"float\"/>\n"
				"              <param name=\"Z\" type=\"float\"/>\n"
				"            </accessor>\n"
				"          </technique_common>\n"
				"        </source>\n", geometry.mesh_name, index, mesh.normalCount);

			//Textures
			fprintf(file,
				"        <source id=\"tex-%s-%d\">\n"
				"          <float_array id=\"texa-%s-%d\" count=\"%d\"> ", geometry.mesh_name, index, geometry.mesh_name, index, mesh.texelCount*2);

			for(int i=0; i < mesh.texelCount; i++)
				fprintf(file, "%f %f ", mesh.texel_array[i].x, mesh.texel_array[i].y);

			fprintf(file, "</float_array>\n"
				"          <technique_common>\n"
				"            <accessor source=\"#texa-%s-%d\" count=\"%d\" stride=\"2\">\n"
				"              <param name=\"S\" type=\"float\"/>\n"
				"              <param name=\"T\" type=\"float\"/>\n"
				"            </accessor>\n"
				"          </technique_common>\n"
				"        </source>\n", geometry.mesh_name, index, mesh.texelCount);

			//Vextex
			fprintf(file,
				"        <vertices id=\"vert-%s-%d\">\n"
				"          <input semantic=\"POSITION\" source=\"#pos-%s-%d\"/>\n"
				"        </vertices>\n", geometry.mesh_name, index, geometry.mesh_name, index);

			//Faces
			fprintf(file,
				"        <triangles count=\"%d\" material=\"material\">\n"
				"          <input semantic=\"VERTEX\" source=\"#vert-%s-%d\" offset=\"0\"/>\n"
				"          <input semantic=\"NORMAL\" source=\"#norm-%s-%d\" offset=\"1\"/>\n"
				"          <input semantic=\"TEXCOORD\" source=\"#tex-%s-%d\" offset=\"2\" set=\"0\"/>\n", mesh.indexCount/3, geometry.mesh_name, index, geometry.mesh_name, index, geometry.mesh_name, index);

			/*fprintf(file, "          <vcount> ");

			for(int i=0; i < mesh.indexCount; i++)
				fprintf(file, "3 ");

			fprintf(file, "</vcount>\n");*/

			fprintf(file, "          <p> ");

			for(int i=0; i < mesh.indexCount; i++)
				fprintf(file, "%d %d %d ", mesh.index_array[i],  mesh.index_array[i],  mesh.index_array[i]);

			fprintf(file, "</p>\n"
				"        </triangles>\n"
				"        <extra>\n"
				"          <technique profile=\"MAYA\">\n"
				"            <double_sided>1</double_sided>\n"
				"          </technique>\n"
				"        </extra>\n"
				"      </mesh>\n"
				"    </geometry>\n");

			index++;
		}
	}
	fprintf(file, "  </library_geometries>\n");

	fprintf(file, "  <library_controllers>\n");
	for(geometryIt = model.geometries.cbegin(), geometryEnd = model.geometries.cend(); geometryIt != geometryEnd; ++geometryIt) {
		const Geometry& geometry = *geometryIt;

		for(index = 0, meshIt = geometry.meshes.cbegin(), meshEnd = geometry.meshes.cend(); meshIt != meshEnd; ++meshIt) {
			const Mesh& mesh = *meshIt;
			size_t boneIndex;

			fprintf(file,
				"    <controller id=\"skin-%s-%d\" name=\"Armature\">\n"
				"      <skin source=\"#geom-%s-%d\">\n", geometry.mesh_name, index, geometry.mesh_name, index);

			//Bones names
			fprintf(file,
				"        <source id=\"bone-%s-%d\">\n"
				"          <Name_array id=\"bonea-%s-%d\" count=\"%d\"> ", geometry.mesh_name, index, geometry.mesh_name, index, mesh.boneVertices.size());

			for(boneIndex = 0; boneIndex < mesh.boneVertices.size(); boneIndex++) {
				const Bone& bone = mesh.boneVertices.at(boneIndex);

				fprintf(file, "%s ", bone.name);
			}

			fprintf(file, "</Name_array>\n"
				"          <technique_common>\n"
				"            <accessor source=\"#bonea-%s-%d\" count=\"%d\" stride=\"1\">\n"
				"              <param name=\"JOINT\" type=\"name\"/>\n"
				"            </accessor>\n"
				"          </technique_common>\n"
				"        </source>\n", geometry.mesh_name, index, mesh.boneVertices.size());


			//Bones weights
			int vertexWeightNum = 0;
			for(boneIndex = 0; boneIndex < mesh.boneVerticesAssociation.size(); boneIndex++) {
				const std::list<VertexBoneAssociation>& vertexInfo = mesh.boneVerticesAssociation.at(boneIndex);
				std::list<VertexBoneAssociation>::const_iterator vertexIt, vertexItEnd;

				for(vertexIt = vertexInfo.cbegin(), vertexItEnd = vertexInfo.cend(); vertexIt != vertexItEnd; ++vertexIt) {
					vertexWeightNum++;
				}
			}
			fprintf(file,
				"        <source id=\"bone-weight-%s-%d\">\n"
				"          <float_array id=\"bone-weighta-%s-%d\" count=\"%d\"> ", geometry.mesh_name, index, geometry.mesh_name, index, vertexWeightNum);

			for(boneIndex = 0; boneIndex < mesh.boneVerticesAssociation.size(); boneIndex++) {
				const std::list<VertexBoneAssociation>& vertexInfo = mesh.boneVerticesAssociation.at(boneIndex);
				std::list<VertexBoneAssociation>::const_iterator vertexIt, vertexItEnd;

				for(vertexIt = vertexInfo.cbegin(), vertexItEnd = vertexInfo.cend(); vertexIt != vertexItEnd; ++vertexIt) {
					fprintf(file, "%f ", vertexIt->weight);
				}
			}

			fprintf(file,
				"</float_array>\n"
				"          <technique_common>\n"
				"            <accessor source=\"#bone-weighta-%s-%d\" count=\"%d\" stride=\"1\">\n"
				"              <param name=\"WEIGHT\" type=\"float\"/>\n"
				"            </accessor>\n"
				"          </technique_common>\n"
				"        </source>\n", geometry.mesh_name, index, vertexWeightNum);

			//Inv matrix
			fprintf(file,
				"        <source id=\"invmat-%s-%d\">\n"
				"          <float_array id=\"invmata-%s-%d\" count=\"%d\"> ", geometry.mesh_name, index, geometry.mesh_name, index, model.boneTransformMatrix.size()*16);

			for(int i = 0; i < model.boneTransformMatrix.size(); i++) {

/*				float invMat[16];

				inverse4x4((float(*)[4])model.boneTransformMatrix.at(i).transformMatrix, (float(*)[4])invMat);

				for(int j = 0; j < 16; j++)
					fprintf(file, "%f ",  invMat[(j*4)%16 + j/4]);
*/
				for(int j = 0; j < 16; j++)
					fprintf(file, "%f ",  model.boneTransformMatrix.at(i).transformMatrix[(j*4)%16 + j/4]);
			}

			fprintf(file, "</float_array>\n"
				"          <technique_common>\n"
				"            <accessor source=\"#invmata-%s-%d\" count=\"%d\" stride=\"16\">\n"
				"              <param name=\"TRANSFORM\" type=\"float4x4\"/>\n"
				"            </accessor>\n"
				"          </technique_common>\n"
				"        </source>\n", geometry.mesh_name, index, model.boneTransformMatrix.size());


			//Joint bones
			fprintf(file,
				"        <joints>\n"
				"          <input semantic=\"JOINT\" source=\"#bone-%s-%d\"/>\n"
				"          <input semantic=\"INV_BIND_MATRIX\" source=\"#invmat-%s-%d\"/>\n"
				"        </joints>\n", geometry.mesh_name, index, geometry.mesh_name, index);

			//Bones indices
			fprintf(file,
				"        <vertex_weights count=\"%d\">\n"
				"          <input semantic=\"JOINT\" source=\"#bone-%s-%d\" offset=\"0\"/>\n"
				"          <input semantic=\"WEIGHT\" source=\"#bone-weight-%s-%d\" offset=\"1\"/>\n"
				"          <vcount> ", mesh.boneVerticesAssociation.size(), geometry.mesh_name, index, geometry.mesh_name, index);
			for(boneIndex = 0; boneIndex < mesh.boneVerticesAssociation.size(); boneIndex++) {
				const std::list<VertexBoneAssociation>& vertexInfo = mesh.boneVerticesAssociation.at(boneIndex);

				fprintf(file, "%d ", vertexInfo.size());
			}
			fprintf(file, "</vcount>\n"
				"          <v> ");

			int vertexWeightIndex = 0;
			for(boneIndex = 0; boneIndex < mesh.boneVerticesAssociation.size(); boneIndex++) {
				const std::list<VertexBoneAssociation>& vertexInfo = mesh.boneVerticesAssociation.at(boneIndex);
				std::list<VertexBoneAssociation>::const_iterator vertexIt, vertexItEnd;

				for(vertexIt = vertexInfo.cbegin(), vertexItEnd = vertexInfo.cend(); vertexIt != vertexItEnd; ++vertexIt) {
					fprintf(file, "%d %d\n", vertexIt->boneIndex, vertexWeightIndex++);
				}
			}
			fprintf(file, "</v>\n"
				"        </vertex_weights>\n");

			fprintf(file,
				"      </skin>\n"
				"    </controller>\n");

			index++;
		}
	}
	fprintf(file, "  </library_controllers>\n");

	/*
	fprintf(file, "  <library_animations>\n");
	for(index = 0, animChannelIt = model.animations.cbegin(), animChannelEnd = model.animations.cend(); animChannelIt != animChannelEnd; ++animChannelIt, index++) {
		const AnimationChannel& animChannel = *animChannelIt;
			int i;
			for(i = 0, boneAnimIt = animChannel.boneAnims.cbegin(), boneAnimEnd = animChannel.boneAnims.cend(); boneAnimIt != boneAnimEnd; ++boneAnimIt, i++) {
					const BoneAnimation& boneAnim = *boneAnimIt;

				//Bone position times
				fprintf(file,
					"    <animation id=\"anim-pos-%d-%s\">\n"
					"      <source id=\"anim-pos-input-%d-%s\">\n"
					"        <float_array id=\"anim-pos-inputa-%d-%s\" count=\"%d\"> ", index, boneAnim.boneName, index, boneAnim.boneName, index, boneAnim.boneName, boneAnim.pos_time_count);

					for(int j = 0; j < boneAnim.pos_time_count; j++)
						fprintf(file, "%f ", boneAnim.pos_time_array[j] / 1000.0);


				fprintf(file, "</float_array>\n"
					"        <technique_common>\n"
					"          <accessor source=\"#anim-pos-inputa-%d-%s\" count=\"%d\" stride=\"1\">\n"
					"            <param name=\"TIME\" type=\"float\"/>\n"
					"          </accessor>\n"
					"        </technique_common>\n"
					"        <technique profile=\"MAYA\">\n"
					"          <pre_infinity>CONSTANT</pre_infinity>\n"
					"          <post_infinity>CONSTANT</post_infinity>\n"
					"        </technique>\n"
					"      </source>\n", index, boneAnim.boneName, boneAnim.pos_time_count);

				//Bone positions vectors
				fprintf(file,
					"      <source id=\"anim-pos-output-%d-%s\">\n"
					"        <float_array id=\"anim-pos-outputa-%d-%s\" count=\"%d\"> ", index, boneAnim.boneName, index, boneAnim.boneName, boneAnim.pos_key_count);

					for(int j = 0; j < boneAnim.pos_key_count; j++)
						fprintf(file, "%f ", boneAnim.pos_key_array[j]);


				fprintf(file, "</float_array>\n"
					"        <technique_common>\n"
					"          <accessor source=\"#anim-pos-outputa-%d-%s\" count=\"%d\" stride=\"3\">\n"
					"            <param name=\"X\" type=\"float\"/>\n"
					"            <param name=\"Y\" type=\"float\"/>\n"
					"            <param name=\"Z\" type=\"float\"/>\n"
					"          </accessor>\n"
					"        </technique_common>\n"
					"      </source>\n", index, boneAnim.boneName, boneAnim.pos_key_count/3);

				//Bone position interpolation
				fprintf(file,
					"      <source id=\"anim-pos-inter-%d-%s\">\n"
					"        <Name_array id=\"anim-pos-intera-%d-%s\" count=\"%d\"> ", index, boneAnim.boneName, index, boneAnim.boneName, boneAnim.pos_time_count);

					for(int j = 0; j < boneAnim.pos_time_count; j++)
						fprintf(file, "LINEAR ");


				fprintf(file, "</Name_array>\n"
					"        <technique_common>\n"
					"          <accessor source=\"#anim-pos-intera-%d-%s\" count=\"%d\" stride=\"1\">\n"
					"            <param name=\"INTERPOLATION\" type=\"Name\"/>\n"
					"          </accessor>\n"
					"        </technique_common>\n"
					"      </source>\n", index, boneAnim.boneName, boneAnim.pos_time_count);

				//Sampler
				fprintf(file,
					"      <sampler id=\"anim-pos-sampler-%d-%s\">\n"
					"        <input semantic=\"INPUT\" source=\"#anim-pos-input-%d-%s\"/>\n"
					"        <input semantic=\"OUTPUT\" source=\"#anim-pos-output-%d-%s\"/>\n"
					"        <input semantic=\"INTERPOLATION\" source=\"#anim-pos-inter-%d-%s\"/>\n"
					"      </sampler>\n", index, boneAnim.boneName, index, boneAnim.boneName, index, boneAnim.boneName, index, boneAnim.boneName);

				//Channel
				fprintf(file,
					"      <channel source=\"#anim-pos-sampler-%d-%s\" target=\"%s/translate\"/>\n"
					"    </animation>\n", index, boneAnim.boneName, boneAnim.boneName);

				/////////////////////////////////////////////////////////////////////////////////////////////
				//Bone rotation times
				fprintf(file,
					"    <animation id=\"anim-rot-%d-%s\">\n"
					"      <source id=\"anim-rot-input-%d-%s\">\n"
					"        <float_array id=\"anim-rot-inputa-%d-%s\" count=\"%d\"> ", index, boneAnim.boneName, index, boneAnim.boneName, index, boneAnim.boneName, boneAnim.rot_time_count);

					for(int j = 0; j < boneAnim.rot_time_count; j++)
						fprintf(file, "%f ", boneAnim.rot_time_array[j] / 1000.0);


				fprintf(file, "</float_array>\n"
					"        <technique_common>\n"
					"          <accessor source=\"#anim-rot-inputa-%d-%s\" count=\"%d\" stride=\"1\">\n"
					"            <param name=\"TIME\" type=\"float\"/>\n"
					"          </accessor>\n"
					"        </technique_common>\n"
					"        <technique profile=\"MAYA\">\n"
					"          <pre_infinity>CONSTANT</pre_infinity>\n"
					"          <post_infinity>CONSTANT</post_infinity>\n"
					"        </technique>\n"
					"      </source>\n", index, boneAnim.boneName, boneAnim.rot_time_count);

				//Bone rotations matrix
				fprintf(file,
					"      <source id=\"anim-rot-output-%d-%s\">\n"
					"        <float_array id=\"anim-rot-outputa-%d-%s\" count=\"%d\"> ", index, boneAnim.boneName, index, boneAnim.boneName, boneAnim.rot_key_count);

					for(int j = 0; j < boneAnim.rot_key_count/4; j++)
						fprintf(file, "%f %f %f %f ",
									boneAnim.rot_key_array[j*4+0] / sqrt(1-pow(boneAnim.rot_key_array[j*4+3], 2)),
									boneAnim.rot_key_array[j*4+1] / sqrt(1-pow(boneAnim.rot_key_array[j*4+3], 2)),
									boneAnim.rot_key_array[j*4+2] / sqrt(1-pow(boneAnim.rot_key_array[j*4+3], 2)),
									2 * acos(boneAnim.rot_key_array[j*4+3]));


				fprintf(file, "</float_array>\n"
					"        <technique_common>\n"
					"          <accessor source=\"#anim-rot-outputa-%d-%s\" count=\"%d\" stride=\"4\">\n"
					"            <param name=\"X\" type=\"float\"/>\n"
					"            <param name=\"Y\" type=\"float\"/>\n"
					"            <param name=\"Z\" type=\"float\"/>\n"
					"            <param name=\"ANGLE\" type=\"float\"/>\n"
					"          </accessor>\n"
					"        </technique_common>\n"
					"      </source>\n", index, boneAnim.boneName, boneAnim.rot_key_count/4);

				//Bone rotation interpolation
				fprintf(file,
					"      <source id=\"anim-rot-inter-%d-%s\">\n"
					"        <Name_array id=\"anim-rot-intera-%d-%s\" count=\"%d\"> ", index, boneAnim.boneName, index, boneAnim.boneName, boneAnim.rot_time_count);

					for(int j = 0; j < boneAnim.rot_time_count; j++)
						fprintf(file, "LINEAR ");


				fprintf(file, "</Name_array>\n"
					"        <technique_common>\n"
					"          <accessor source=\"#anim-rot-intera-%d-%s\" count=\"%d\" stride=\"1\">\n"
					"            <param name=\"INTERPOLATION\" type=\"Name\"/>\n"
					"          </accessor>\n"
					"        </technique_common>\n"
					"      </source>\n", index, boneAnim.boneName, boneAnim.rot_time_count);

				//Sampler
				fprintf(file,
					"      <sampler id=\"anim-rot-sampler-%d-%s\">\n"
					"        <input semantic=\"INPUT\" source=\"#anim-rot-input-%d-%s\"/>\n"
					"        <input semantic=\"OUTPUT\" source=\"#anim-rot-output-%d-%s\"/>\n"
					"        <input semantic=\"INTERPOLATION\" source=\"#anim-rot-inter-%d-%s\"/>\n"
					"      </sampler>\n", index, boneAnim.boneName, index, boneAnim.boneName, index, boneAnim.boneName, index, boneAnim.boneName);

				//Channel
				fprintf(file,
					"      <channel source=\"#anim-rot-sampler-%d-%s\" target=\"%s/rotate\"/>\n"
					"    </animation>\n", index, boneAnim.boneName, boneAnim.boneName);

			}
	}

	fprintf(file, "  </library_animations>\n");
	*/

	fprintf(file,
		"  <library_visual_scenes>\n"
		"    <visual_scene id=\"Scene\" name=\"Scene\">\n");

	fprintf(file,
		"      <node id=\"Armature\" name=\"Armature\" type=\"NODE\">\n");

	for(index = 0; index < model.boneTransformMatrix.size(); index++) {
		fprintf(file,
			"        <node id=\"%s\" name=\"%s\" sid=\"%s\" type=\"JOINT\">\n"
			"          <matrix sid=\"transform\"> ", model.boneTransformMatrix.at(index).name, model.boneTransformMatrix.at(index).name, model.boneTransformMatrix.at(index).name);

		float invMat[16];

		inverse4x4((float(*)[4])model.boneTransformMatrix.at(index).transformMatrix, (float(*)[4])invMat);

		for(int j = 0; j < 16; j++)
			fprintf(file, "%f ",  invMat[(j*4)%16 + j/4]);

		//for(int j = 0; j < 16; j++)
		//	fprintf(file, "%f ",  model.boneTransformMatrix.at(index).transformMatrix[(j*4)%16 + j/4]);

		fprintf(file,
			"</matrix>\n"
		//	"          <translate sid=\"translate\">0 0 0</translate>\n"
		//	"          <rotate sid=\"rotate\">1 0 0 0</rotate>\n"
			"        </node>\n");
	}
	fprintf(file, "      </node>\n");


	for(geometryIt = model.geometries.cbegin(), geometryEnd = model.geometries.cend(); geometryIt != geometryEnd; ++geometryIt) {
		const Geometry& geometry = *geometryIt;

		for(index = 0, meshIt = geometry.meshes.cbegin(), meshEnd = geometry.meshes.cend(); meshIt != meshEnd; ++meshIt) {
			const Mesh& mesh = *meshIt;


			fprintf(file,
				"      <node id=\"node-%s-%d\" name=\"%s.%d\" type=\"NODE\">\n"
				"        <translate sid=\"location\">0 0 0</translate>\n"
				"        <rotate sid=\"rotationZ\">0 0 1 0</rotate>\n"
				"        <rotate sid=\"rotationY\">0 1 0 0</rotate>\n"
				"        <rotate sid=\"rotationX\">1 0 0 0</rotate>\n"
				"        <scale sid=\"scale\">1 1 1</scale>\n"
				"        <instance_controller url=\"#skin-%s-%d\">\n", geometry.mesh_name, index, geometry.mesh_name, index, geometry.mesh_name, index);


			for(int i = 0; i < model.boneTransformMatrix.size(); i++) {
				fprintf(file, "          <skeleton>#%s</skeleton>\n",  model.boneTransformMatrix.at(i).name);
			}

			fprintf(file,
				"          <bind_material>\n"
				"            <technique_common>\n"
				"              <instance_material symbol=\"material\" target=\"#mat-%d\">\n"
				"                <bind_vertex_input semantic=\"UVMap\" input_semantic=\"TEXCOORD\" input_set=\"0\"/>\n"
				"              </instance_material>\n"
				"            </technique_common>\n"
				"          </bind_material>\n"
				"        </instance_controller>\n", mesh.textureId);

			fprintf(file,
				"      </node>\n");
			index++;
		}
	}

	fprintf(file,
		"    </visual_scene>\n"
		"  </library_visual_scenes>\n"
		"  <scene>\n"
		"    <instance_visual_scene url=\"#Scene\"/>\n"
		"  </scene>\n"
		"</COLLADA>\n");
}

void parseFunc(IBtrfRootBlock* rootBlock, IBtrfRootBlock* animRootBlock, FILE* file) {

	GUID nx3_mtl_header_guid = {0x209BBB41, 0x681F, 0x4b9b, {0x97, 0x44, 0x4D, 0x88, 0xE1, 0x41, 0x3D, 0xCC}};
	GUID nx3_new_mesh_header = {0xA6D25AEB, 0xA735, 0x1FEF, {0xC1, 0x7D, 0xEE, 0x21, 0x17, 0x49, 0x82, 0x26}};
	GUID nx3_mesh_header =     {0xD6D25AEB, 0xA735, 0x4fef, {0xA1, 0x7D, 0x6E, 0x21, 0x17, 0x49, 0x82, 0x26}};
	GUID nx3_bone_ani_header = {0xE8F9296B, 0xB9DD, 0x4080, {0x8B, 0xC7, 0x6C, 0x69, 0xE0, 0xAA, 0x3F, 0xEB}};

	Model model;
	Material material;
	Geometry geometry;
	Mesh mesh;

	IBtrfBlock *currentBlock;

	currentBlock = rootBlock->getBlock(nx3_mtl_header_guid);
	if(currentBlock) {
		currentBlock = currentBlock->getBlock(0)->getBlock(0)->	//mtl_array
							getBlock(0);	//sub_mtl_block_array

		int i;
		for(i = 0; i < currentBlock->getElementNumber(); i++) {
			IBtrfBlock *materialInfo = currentBlock->getBlock(i);

			material.mtl_name =     materialInfo->getBlock(0)->getDataString(0);
			material.texture_name = materialInfo->getBlock(1)->getDataString(0);
			material.mtl_id =       materialInfo->getBlock(2)->getDataInt(0);
			material.power =        materialInfo->getBlock(4)->getDataFloat(0);
			material.self_illumi =  materialInfo->getBlock(5)->getDataFloat(0);
			material.ambient =      materialInfo->getBlock(7)->getDataInt(0);
			material.diffuse =      materialInfo->getBlock(8)->getDataInt(0);
			material.specular =     materialInfo->getBlock(9)->getDataInt(0);
			model.materials.push_back(material);
		}
	}

	currentBlock = rootBlock->getBlock(nx3_new_mesh_header);
	if(currentBlock == 0)
		currentBlock = rootBlock->getBlock(nx3_mesh_header);
	if(currentBlock) {
		IBtrfBlock *subBlock = currentBlock->getBlock(0);

		int i;
		for(i = 0; i < subBlock->getElementNumber(); i++) {
			IBtrfBlock *geometryInfo = subBlock->getBlock(i);
			IBtrfBlock *meshInfo;

			geometry.mesh_name = geometryInfo->getBlock(0)->getDataString(0);
			geometry.material_id = geometryInfo->getBlock(1)->getDataInt(0);
			geometry.channel_id = geometryInfo->getBlock(2)->getDataInt(0);

			meshInfo = geometryInfo->getBlock(3);
			for(int j = 0; j < meshInfo->getElementNumber(); j++) {
				IBtrfBlock *meshBlock = meshInfo->getBlock(j);
				IBtrfBlock *meshData = meshBlock->getBlock(1)->getBlock(0);

				mesh.textureId = meshBlock->getBlock(0)->getDataInt(0);
				mesh.vertexCount = meshData->getBlock(1)->getElementNumber()/3;
				mesh.vertex_array = (Vector3D*)meshData->getBlock(1)->getDataFloatPtr();
				mesh.normalCount = meshData->getBlock(2)->getElementNumber()/3;
				mesh.normal_array = (Vector3D*)meshData->getBlock(2)->getDataFloatPtr();
				mesh.texelCount = meshData->getBlock(3)->getElementNumber()/2;
				mesh.texel_array = (Vector2D*)meshData->getBlock(3)->getDataFloatPtr();
				mesh.indexCount = meshBlock->getBlock(2)->getElementNumber();
				mesh.index_array = meshBlock->getBlock(2)->getDataShortPtr();

				IBtrfBlock *boneInfo = meshData->getBlock(5);

				mesh.boneVerticesAssociation.clear();
				mesh.boneVerticesAssociation.resize(mesh.vertexCount);
				mesh.boneVertices.clear();
				mesh.boneVertices.resize(boneInfo->getElementNumber());
				int k;
				for(k = 0; k < boneInfo->getElementNumber(); k++) {
					Bone& bone = mesh.boneVertices[k];

					bone.name = boneInfo->getBlock(k)->getBlock(0)->getDataString(0);
					bone.verticeCount = boneInfo->getBlock(k)->getBlock(1)->getElementNumber()/2;
					bone.boneVertex = (Bone::VertexInfluence*)boneInfo->getBlock(k)->getBlock(1)->getDataFloatPtr();
					bone.vertexOffset = (Vector3D*)boneInfo->getBlock(k)->getBlock(2)->getDataFloatPtr();

					for(int l = 0; l < bone.verticeCount; ++l) {
						VertexBoneAssociation assoc;
						int vertexIndex = bone.boneVertex[l].vertexIndex;

						assoc.boneIndex = k;
						assoc.weight = bone.boneVertex[l].weight;
						mesh.boneVerticesAssociation[vertexIndex].push_back(assoc);
					}
				}
				model.boneTransformMatrix.resize(std::max(k, (int)model.boneTransformMatrix.size()));

				mesh.transformMatrix = meshData->getBlock(6)->getDataFloatPtr();

				geometry.meshes.push_back(mesh);
			}

			model.geometries.push_back(geometry);
		}

		//Bones matrix
		subBlock = currentBlock->getBlock(1);
		for(i = 0; i < subBlock->getElementNumber(); i++) {
			IBtrfBlock *boneBlock = subBlock->getBlock(i);

			model.boneTransformMatrix[i].name = boneBlock->getBlock(0)->getDataString(0);
			model.boneTransformMatrix[i].transformMatrix = boneBlock->getBlock(1)->getDataFloatPtr();
		}
	}

/*
	currentBlock = animRootBlock->getBlock(nx3_bone_ani_header);
	if(currentBlock) {
		BtrfBlock *subBlock = currentBlock->getBlock(1);

		int i;
		for(i = 0; i < subBlock->getElementNumber(); i++) {
			BtrfBlock *channelBlock = subBlock->getBlock(i);
			AnimationChannel animationModel;

			for(int j = 0; j < channelBlock->getBlock(4)->getElementNumber(); j++) {
				BtrfBlock *boneBlock = channelBlock->getBlock(4)->getBlock(j);
				BoneAnimation boneAnim;

				boneAnim.boneName = boneBlock->getBlock(0)->getData<const char*>(0);

				boneAnim.pos_time_count = boneBlock->getBlock(4)->getElementNumber();
				boneAnim.pos_time_array = boneBlock->getBlock(4)->getDataPtr<int*>();
				boneAnim.pos_key_count = boneBlock->getBlock(5)->getElementNumber();
				boneAnim.pos_key_array = boneBlock->getBlock(5)->getDataPtr<float*>();

				boneAnim.rot_time_count = boneBlock->getBlock(6)->getElementNumber();
				boneAnim.rot_time_array = boneBlock->getBlock(6)->getDataPtr<int*>();
				boneAnim.rot_key_count = boneBlock->getBlock(7)->getElementNumber();
				boneAnim.rot_key_array = boneBlock->getBlock(7)->getDataPtr<float*>();

				animationModel.boneAnims.push_back(boneAnim);
			}

			model.animations.push_back(animationModel);
		}
	}
	*/
	//writeObj(texturePath, vertexArraySize, vertexArray, texCoordArraySize, texCoordArray, normalsArraySize, normalsArray, indicesArraySize, indicesArray);
	writeCollada(model, file);
}

void help();

int main(int argc, char* argv[])
{
	FILE* tml_file;
	FILE* outfile;

	ITmlFile *tmlFile;
	IBtrfRootBlock *rootBlock;
	int i;

	std::deque<const char*> templateFiles;
	std::deque<const char*> inputFiles;
	std::deque<IBtrfRootBlock*> rootBlocks;
	const char* outFile = 0;
	const char* outBtrfFile = 0;
	bool dumpData = false;

	if(argc <= 1)
		help();


	for(i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "--input") && (i+1) < argc) {
			inputFiles.push_back(argv[i+1]);
			i++;
		} else if(!strcmp(argv[i], "--template") && (i+1) < argc) {
			templateFiles.push_back(argv[i+1]);
			i++;
		} else if(!strcmp(argv[i], "--output") && (i+1) < argc) {
			outFile = argv[i+1];
			i++;
		} else if(!strcmp(argv[i], "--output-btrf") && (i+1) < argc) {
			outBtrfFile = argv[i+1];
			i++;
		} else if(!strcmp(argv[i], "--dump")) {
			dumpData = true;
		} else if(!strcmp(argv[i], "--help")) {
			help();
		}
	}

	tmlFile = createTmlFile();

	if(templateFiles.size() == 0) {
		templateFiles.push_back("nx3.tml");
		templateFiles.push_back("nobj.tml");
	}

	for(i = 0; i < templateFiles.size(); i++) {
		tmlFile->parseFile(templateFiles.at(i));
	}

	IBtrfParser *parser = createBtrfParser(tmlFile);

	for(i = 0; i < inputFiles.size(); i++) {
		rootBlock = parser->readFile(inputFiles.at(i));
		if(!rootBlock) {
			fprintf(stderr, "Warning: cannot open input file %s\n", inputFiles.at(i));
			continue;
		}
		rootBlocks.push_back(rootBlock);
	}

	if(dumpData) {
		for(i = 0; i < rootBlocks.size(); i++) {
			rootBlocks.at(i)->dumpToStdout();
		}
	}

	fflush(stdout);

	if(outBtrfFile && rootBlocks.size() >= 1) {
		parser->writeFile(outBtrfFile, rootBlocks.at(0));
	}

	if(outFile && rootBlocks.size() >= 1) {
		outfile = fopen(outFile, "w");
		parseFunc(rootBlocks.at(0), 0, outfile);
		fclose(outfile);
	}

    return 0;
}

void help() {
	printf("BTRFReader by Glandu2 - Read BTRF files, dump and export to collada nx3 files\n"
		   "Usage: BTRFReader.exe --input <input_file> [--input <input_file> ...]\n"
		   "           [--template <template_file.tml> [--template <template_file.tml> ...]]\n"
		   "           [--output <output_file.dae>] [--dump]\n"
		   "\n"
		   "--input       Specify which input files to read. These files are usually\n"
		   "                  *.cob, *.nx3, *.naf\n"
		   "--template    Specify custom template files. When not specified,\n"
		   "                  default are used (nx3.tml & nobj.tml)\n"
		   "--ouput       The output collada file. The first input file MUST be a .nx3 file\n"
		   "--dump        Dump the contents of the files to stdout\n"
		   "                  (ie: to the console when not redirected)\n"
		   "\n"
		   " Note:\n"
		   " - Currently, only the mesh and bone data export to collada are supported\n"
		   "     Export of animations is not supported.\n"
		   " - Importing model from collada to BTRF is not supported, it\'s only a reader.\n");

	exit(0);
}

volatile const char* signature = "\x05\xA7\xDE\x20\x2F\x2F\x62\x7B\xC3\xEF\x96\x8C\x70\xFC\x85\x04\x84\xC0\x00\x34\x91\xBD\x91\x80\x91\xCF\xA5\x75\x79\x3C\x4C\x69\x91\x07\x91\x9B\x4C\xB3\x51\x08\x4A\xDC\xAD\x87\x97\x25\x97\x92";
