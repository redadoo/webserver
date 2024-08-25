#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <HttpMessage.hpp>
# include <HttpStatusCode.hpp>

class HttpResponse : public HttpMessage
{
public:
    HttpStatusCode::Code code;

};

#endif