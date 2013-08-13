#ifndef GUID_H
#define GUID_H

typedef union {
	struct {
		unsigned int  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[ 8 ];
	};
	unsigned int rawData[4];
} Guid;

inline bool operator==(const Guid& guidOne, const Guid& guidOther)
	{ return
			(guidOne.rawData[0] == guidOther.rawData[0]) &&
			(guidOne.rawData[1] == guidOther.rawData[1]) &&
			(guidOne.rawData[2] == guidOther.rawData[2]) &&
			(guidOne.rawData[3] == guidOther.rawData[3]);
	}
inline bool operator!=(const Guid& g1, const Guid& g2)
	{ return !(g1 == g2); }

#endif // GUID_H
