FROM cern/cc7-base

LABEL "maintainer"="Felipe Silva"

RUN curl https://ipbus.web.cern.ch/doc/user/html/_downloads/ipbus-sw.centos7.x86_64.repo -o /etc/yum.repos.d/ipbus-sw.repo

RUN yum groupinstall -y 'Development Tools' uhal  

RUN yum install -y git vim wget lynx

RUN yum install -y python-pip
RUN pip install --upgrade pip
RUN pip install pybind11 cmake
RUN yum install -y python-devel
RUN yum install -y epel-release
RUN yum install -y root

RUN mkdir /gif_daq 

WORKDIR /gif_daq 

ENV LD_LIBRARY_PATH /opt/cactus/lib:$LD_LIBRARY_PATH
ENV PATH /opt/cactus/bin:$PATH

ENTRYPOINT ["/bin/bash"]
