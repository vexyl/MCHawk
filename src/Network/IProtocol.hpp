// TODO: Move to a Protocols folder
// FIXME: Not used yet

#ifndef IPROTOCOL_H_
#define IPROTOCOL_H_

#include <string>

class IProtocol {
	virtual ~IProtocol();

	virtual bool IsValidBlock(int type) = 0;
	virtual std::string GetName() = 0;
};

#endif // IPROTOCOL_H_
