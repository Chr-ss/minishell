#https://stackoverflow.com/questions/70852648/remove-docker-image-if-it-exists


check_image=$(docker images -af reference='minishell/tester' -q)
check_container=$(docker ps -a | grep 'minishell/tester' | awk '{print $1}')


if [ -z "$check_container" ];
then :
else docker rm $check_container
fi

if [ -z "$check_image" ];
then :
else docker rmi $check_image
fi

docker build -t minishell/tester .

docker run -it minishell/tester
