#ifndef EPOLLUTILS
# define EPOLLUTILS

# include <sys/epoll.h>
# include <arpa/inet.h>
# include <netinet/tcp.h>
# include <cstring>

namespace EpollUtils
{
    /// @brief  Add file descriptor to epoll instance
    void EpollAdd(int epollFd, int fd, uint32_t events);

    /// @brief  Remove file descriptor to epoll instance
    void EpollDelete(int epoll_fd, int fd);

    /// @brief  creates an instance of epoll and returns its file descriptor
    int EpollInit();

} // namespace EpollUtils



#endif