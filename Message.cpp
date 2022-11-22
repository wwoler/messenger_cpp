#include "Message.h"

Message::Message(const User & whom, const std::string & toWhom, const std::string message):_fromWhom(std::make_shared<User>(whom)), _toWhom(toWhom), _message(message){}