#include <EpollUtils.hpp>

void EpollUtils::EpollAdd(int epollFd, int fd, uint32_t events)
{
	struct epoll_event event;

	/* Shut the valgrind up! */
	memset(&event, 0, sizeof(struct epoll_event));

	event.events  = events;
	event.data.fd = fd;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) < 0) 
		throw WebServerException::ExceptionErrno("epoll_ctl(EPOLL_CTL_ADD): ", errno);
}

void EpollUtils::EpollDelete(int epoll_fd, int fd)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0) 
		throw WebServerException::ExceptionErrno("epoll_ctl(EPOLL_CTL_DEL): ", errno);
}