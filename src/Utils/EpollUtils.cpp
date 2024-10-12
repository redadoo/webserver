#include <EpollUtils.hpp>
#include <Logger.hpp>
#include <StringUtils.hpp>
#include <WebServerException.hpp>


void EpollUtils::EpollAdd(int epollFd, int fd, uint32_t events)
{
	struct epoll_event event;

	memset(&event, 0, sizeof(struct epoll_event));

	event.events  = events;
	event.data.fd = fd;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) < 0)
		throw WebServerException::ExceptionErrno("epoll_ctl(EPOLL_CTL_ADD): ", errno);

	Logger::Log("add file descriptor to epoll instance ...");
}

void EpollUtils::EpollDelete(int epoll_fd, int fd)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0)
		Logger::LogError("Failed to remove file descriptor " + StringUtils::ToString(fd) + " from epoll");
	else
		Logger::Log("Removed file descriptor " + StringUtils::ToString(fd) + " from epoll instance");
}

bool EpollUtils::EpollCheckEventError(uint32_t events)
{
	return (events & EPOLLERR) || (events & EPOLLHUP);
}

int EpollUtils::EpollInit()
{
	Logger::Log("start create a new epoll instance");

	int epollFd;

	epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw WebServerException::ExceptionErrno("epoll_create() failed ", errno);

	Logger::Log("init epoll instance successfully");

	return epollFd;
}
