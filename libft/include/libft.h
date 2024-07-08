/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/03 15:23:23 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/08 14:55:35 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <unistd.h>
# include <stdarg.h>
# include <limits.h>
# include <unistd.h>

// LIBFT BONUS STRUCT
typedef struct s_libft
{
	void			*content;
	struct s_libft	*next;
}	t_libft;

// GET_NEXT_LINE STRUCT
typedef struct s_gnl
{
	char			*buffer;
	struct s_gnl	*next;
}	t_gnl;

// FT_PRINTF STRUCT
typedef struct s_flags
{
	int	spec;
	int	width;
	int	star;
	int	zero;
	int	minus;
	int	dot;
	int	hash;
	int	space;
	int	plus;
}	t_flags;

// GET_NEXT_LINE DEFINE BUFFER_SIZE
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50000
# endif

// LIBFT FUNCTIONS
// ft_convert..
char	**ft_split(char const *s, char c);
int		ft_atoi(const char *nptr);
char	*ft_itoa(int n);
int		ft_tolower(int c);
int		ft_toupper(int c);
// ft_istype
int		ft_isdigit(int c);
int		ft_isspace(int c);
int		ft_isalnum(int c);
int		ft_isupper(int c);
int		ft_islower(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
// ft_list..
t_libft	*ft_lstnew(void *content);
void	ft_lstadd_front(t_libft **lst, t_libft *lstnew);
int		ft_lstsize(t_libft *lst);
t_libft	*ft_lstlast(t_libft *lst);
void	ft_lstadd_back(t_libft **lst, t_libft *lstnew);
void	ft_lstdelone(t_libft *lst, void (*del)(void*));
void	ft_lstiter(t_libft *lst, void (*f)(void *));
void	ft_lstclear(t_libft **lst, void (*del)(void*));
t_libft	*ft_lstmap(t_libft *lst, void *(*f)(void *), void (*del)(void *));
// ft_melloc..
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free_strarr(char **strarr);
char	*ft_dynstralloc(char *original_str, int *capacity);
// ft_mem..
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
// ft_put..
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
// ft_str..
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strdup(const char *s);
char	*ft_strndup(const char *s, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));

// FT_PRINTF FUNCTIONS
int		ft_printf(const char *format, ...);
// hex.c functions for hex conversions for flags: x X p
void	printf_x_upper(unsigned int n, unsigned int *count, t_flags flags);
void	printf_x(unsigned int n, unsigned int *count, t_flags flags);
void	printf_p(unsigned long n, unsigned int *count);
void	printf_get_hex(unsigned long n, char *number);
// chars.c functions for char and string for flags: c s
int		printf_s(char *s, unsigned int *count);
int		printf_c(int i, unsigned int *count);
// numbers.c functions for nubmers for flags: i d u
void	printf_int(int n, unsigned int *count, t_flags flags);
void	printf_u(unsigned int n, unsigned int *count);
void	printf_int_digits(int n, unsigned int *count, int len, t_flags flags);

// GET_NEXT_LINE FUNCTIONS
char	*get_next_line(int fd);

int		gnl_lst_until_nl(t_gnl *lst);
int		gnl_lst_until_nl1(t_gnl *lst);
void	*gnl_ft_calloc(size_t nmemb, size_t size);
void	gnl_lst_to_next_line(t_gnl *lst, char *next_line);
int		gnl_free_master(t_gnl **list, t_gnl **clean_node,
			char **buf, char **nl);

#endif /* LIBFT_H */
