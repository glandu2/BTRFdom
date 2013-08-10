#ifndef DIFK_UNKNOWNMAGICEXCEPTION_H
#define DIFK_UNKNOWNMAGICEXCEPTION_H

#include <exception>

namespace DIFK {

class UnknownMagicException : public std::exception
{
	public:
		UnknownMagicException();
	protected:
	private:
};

} // namespace DIFK

#endif // DIFK_UNKNOWNMAGICEXCEPTION_H
