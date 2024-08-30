#ifndef HTTPSTATUSCODE
# define HTTPSTATUSCODE

namespace HttpStatusCode
{
	enum Code
	{
		toSet 							= 0,
		Continue            			= 100,
		SwitchingProtocols  			= 101,
		Processing          			= 102,
		EarlyHints          			= 103,
		OK                          	= 200,
		Created                     	= 201,
		Accepted                    	= 202,
		NonAuthoritativeInformation 	= 203,
		NoContent                  	 	= 204,
		ResetContent                	= 205,
		PartialContent              	= 206,
		MultiStatus                 	= 207,
		AlreadyReported        	    	= 208,
		IMUsed                  	    = 226,
		MultipleChoices   				= 300,
		MovedPermanently  				= 301,
		Found             				= 302,
		SeeOther          				= 303,
		NotModified       				= 304,
		UseProxy          				= 305,
		TemporaryRedirect 				= 307,
		PermanentRedirect 				= 308,
		BadRequest                  	= 400,
		Unauthorized                	= 401,
		PaymentRequired             	= 402,
		Forbidden                   	= 403,
		NotFound                    	= 404,
		MethodNotAllowed            	= 405,
		NotAcceptable               	= 406,
		ProxyAuthenticationRequired 	= 407,
		RequestTimeout              	= 408,
		Conflict                    	= 409,
		Gone                        	= 410,
		LengthRequired              	= 411,
		PreconditionFailed          	= 412,
		ContentTooLarge             	= 413,
		PayloadTooLarge             	= 413,
		URITooLong                  	= 414,
		UnsupportedMediaType        	= 415,
		RangeNotSatisfiable         	= 416,
		ExpectationFailed           	= 417,
		MisdirectedRequest          	= 421,
		UnprocessableContent        	= 422,
		UnprocessableEntity         	= 422,
		Locked                      	= 423,
		FailedDependency            	= 424,
		TooEarly                    	= 425,
		UpgradeRequired             	= 426,
		PreconditionRequired        	= 428,
		TooManyRequests             	= 429,
		RequestHeaderFieldsTooLarge 	= 431,
		UnavailableForLegalReasons  	= 451,
		InternalServerError           	= 500,
		NotImplemented                	= 501,
		BadGateway                    	= 502,
		ServiceUnavailable            	= 503,
		GatewayTimeout                	= 504,
		HTTPVersionNotSupported       	= 505,
		VariantAlsoNegotiates         	= 506,
		InsufficientStorage           	= 507,
		LoopDetected                  	= 508,
		NotExtended                   	= 510,
		NetworkAuthenticationRequired 	= 511,
		xxx_max = 1023
	};

	inline bool isInformational(int code) { return (code >= 100 && code < 200); }
	inline bool isSuccessful(int code)    { return (code >= 200 && code < 300); }
	inline bool isRedirection(int code)   { return (code >= 300 && code < 400); }
	inline bool isClientError(int code)   { return (code >= 400 && code < 500); }
	inline bool isServerError(int code)   { return (code >= 500 && code < 600); }
	inline bool isError(int code)         { return (code >= 400); }


	inline const std::string reasonPhrase(int code)
	{
		switch (code)
		{

		//####### 1xx - Informational #######
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 103: return "Early Hints";

		//####### 2xx - Successful #######
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";
		case 226: return "IM Used";

		//####### 3xx - Redirection #######
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";

		//####### 4xx - Client Error #######
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Content Too Large";
		case 414: return "URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Content";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 425: return "Too Early";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 451: return "Unavailable For Legal Reasons";

		//####### 5xx - Server Error #######
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";

		default: return std::string();
		}
	}

	inline std::string reasonPhrase(Code code)
    {
        switch (code)
        {
            // 1xx Informational
            case Continue:            return "100 Continue";
            case SwitchingProtocols:  return "101 Switching Protocols";
            case Processing:          return "102 Processing";

            // 2xx Success
            case OK:                  return "200 OK";
            case Created:             return "201 Created";
            case Accepted:            return "202 Accepted";
            case NoContent:           return "204 No Content";
            case ResetContent:        return "205 Reset Content";
            case PartialContent:      return "206 Partial Content";

            // 3xx Redirection
            case MultipleChoices:     return "300 Multiple Choices";
            case MovedPermanently:    return "301 Moved Permanently";
            case Found:               return "302 Found";
            case SeeOther:            return "303 See Other";
            case NotModified:         return "304 Not Modified";
            case TemporaryRedirect:   return "307 Temporary Redirect";
            case PermanentRedirect:   return "308 Permanent Redirect";

            // 4xx Client Error
            case BadRequest:          return "400 Bad Request";
            case Unauthorized:        return "401 Unauthorized";
            case Forbidden:           return "403 Forbidden";
            case NotFound:            return "404 Not Found";
            case MethodNotAllowed:    return "405 Method Not Allowed";
            case NotAcceptable:       return "406 Not Acceptable";
            case RequestTimeout:      return "408 Request Timeout";
            case Conflict:            return "409 Conflict";
            case Gone:                return "410 Gone";
            case LengthRequired:      return "411 Length Required";
            case PayloadTooLarge:     return "413 Payload Too Large";
            case URITooLong:          return "414 URI Too Long";
            case UnsupportedMediaType:return "415 Unsupported Media Type";
            case RangeNotSatisfiable: return "416 Range Not Satisfiable";
            case ExpectationFailed:   return "417 Expectation Failed";
            case UnprocessableEntity: return "422 Unprocessable Entity";
            case TooManyRequests:     return "429 Too Many Requests";

            // 5xx Server Error
            case InternalServerError: return "500 Internal Server Error";
            case NotImplemented:      return "501 Not Implemented";
            case BadGateway:          return "502 Bad Gateway";
            case ServiceUnavailable:  return "503 Service Unavailable";
            case GatewayTimeout:      return "504 Gateway Timeout";
            case HTTPVersionNotSupported: return "505 HTTP Version Not Supported";

            default:                  return "500 Internal Server Error";
        }
	}

} // namespace HttpStatusCode

#endif
