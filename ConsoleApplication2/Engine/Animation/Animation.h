#pragma once
#include <string>

class Animation {
protected:
	std::string id;

public:
	const std::string&	GetId(void);
	void				SetId(const std::string&);
	virtual Animation*	Clone(void) const = 0;
	Animation(const std::string&);
	virtual ~Animation(void);
};

