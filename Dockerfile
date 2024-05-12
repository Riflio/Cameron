# @brief Образ для сборки Cameron
#
# Собрать образ контейнера: sudo docker build --no-cache -t cameron .
# Запустить контейнер (и сборку содержимого): sudo docker run --mount type=bind,source=`pwd`/.,target=/APP --rm -e SOURCE_DATE_EPOCH=1662635002 -t cameron:latest  #--Вместо pwd подставить $WORKSPACE для сборки на Jenkins
# Войти в шелл не запущенного контейнера: sudo docker run -it --entrypoint /bin/bash --mount type=bind,source=`pwd`/.,target=/APP cameron
# Войти в шелл запущенного контейнера: sudo docker exec -it cameron /bin/bash
#
# Get install folder from image to host
# id=$(sudo docker create licensingserver:latest)
# sudo docker cp $id:/opt/Qt/Qt630/install_LinAmd64 /opt/Qt/Qt630/
# sudo docker rm -v $id
#
# @author 2me@pavelk.ru

FROM ubuntu:18.04
LABEL maintainer="2me@pavelk.ru"

ENV DEBIAN_FRONTEND=noninteractive
ENV SOURCE_DATE_EPOCH=1662549182 

# Ext packages
COPY Dist /tmp

# RUN
RUN apt-get update && \
# Tools
apt install -y wget build-essential software-properties-common && \
# GCC&G++ 9.4
add-apt-repository ppa:ubuntu-toolchain-r/test -y &&\
apt install -y gcc-9 g++-9 && \
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 10 && \
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 10 && \
# OpenSSL 1.1
apt install -y libssl1.1 libssl-dev && \
dpkg -i /tmp/libssl1.1_1.1.1-1ubuntu2.1~18.04.15_amd64.deb && \
dpkg -i /tmp/libssl-dev_1.1.1-1ubuntu2.1~18.04.15_amd64.deb && \
# CMake 3.22.2
dpkg -i /tmp/cmake_3.22.2-1_amd64.deb && \
# Build Qt
mkdir -p /opt/Qt/Qt640/src && tar -xf /tmp/Qt640.tar.gz -C /opt/Qt/Qt640/src  && \
chmod +x /opt/Qt/Qt640/src/BUILD_QT.sh && \
chmod +x /opt/Qt/Qt640/src/BUILD_QT.sh && /opt/Qt/Qt640/src/BUILD_QT.sh 


# Build App
#CMD /APP/BUILD_APP.sh1