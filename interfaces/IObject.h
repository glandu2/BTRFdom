#ifndef IOBJECT_H
#define IOBJECT_H

#include "ExportDLL.h"
#include "Guid.h"

class IUnknown;

// {00000000-0000-0000-C000-000000000046}
static const Guid COM_IID_IUnknown =
{{ 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } }};

#define COM_NOERROR 0
#define COM_E_UNEXPECTED ((long)0x8000FFFFL)
#define COM_E_NOTIMPL ((long)0x80004001L)
#define COM_E_OUTOFMEMORY ((long)0x8007000EL)
#define COM_E_INVALIDARG ((long)0x80070057L)
#define COM_E_NOINTERFACE ((long)0x80004002L)
#define COM_E_POINTER ((long)0x80004003L)
#define COM_E_HANDLE ((long)0x80070006L)
#define COM_E_ABORT ((long)0x80004004L)
#define COM_E_FAIL ((long)0x80004005L)
#define COM_E_ACCESSDENIED ((long)0x80070005L)
#define COM_E_PENDING ((long)0x8000000AL)


#define COM_DECLARE_IFACE(iface) \
	if (riid == IID_##iface)                                                    \
	{                                                                           \
		*ppObj = (iface*)this;                                                  \
		AddRef();                                                               \
		return COM_NOERROR;                                                     \
	}

#define COM_BEGIN_DECLARE_IFACE \
	virtual inline long DLLCALLCONV QueryInterface(const Guid& riid , void **ppObj) {

#define COM_END_DECLARE_IFACE \
		if (riid == COM_IID_IUnknown)                                               \
		{                                                                           \
			*ppObj = (IUnknown*)this;                                               \
			AddRef();                                                               \
			return S_OK;                                                            \
		}                                                                           \
		*ppObj = NULL;                                                              \
		return COM_E_NOINTERFACE;                                                   \
	}


class IObject;

class IObject
{
	public:
		virtual long DLLCALLCONV QueryInterface(const Guid& iid, void** ppv) = 0;
		virtual unsigned long DLLCALLCONV AddRef() = 0;
		virtual unsigned long DLLCALLCONV Release() = 0;
		virtual void DLLCALLCONV destroy() = 0;

		void operator delete(void* p) {
			if (p) {
				IObject* i = static_cast<IObject*>(p);
				i->destroy();
			}
		}
};

template<class IInterface, Guid* IID_IFace = nullptr>
class CImplement : public IInterface
{
	public:
		CImplement() : refCount(1) {}
		virtual ~CImplement() {}
		void operator delete(void* p) {
			::operator delete(p);
		}

		virtual inline long DLLCALLCONV QueryInterface(const Guid& iid, void** ppv) {
			return COM_E_NOINTERFACE;
		}

		virtual inline unsigned long DLLCALLCONV AddRef() { return ++refCount; }

		virtual inline unsigned long DLLCALLCONV Release() {
			refCount--;
			if(!refCount) {
				destroy();
				return 0;
			}
			return refCount;
		}

		virtual void DLLCALLCONV destroy() {
			delete this;
		}


	private:
		int refCount;
};

#endif // IOBJECT_H
