FROM mcr.microsoft.com/devcontainers/cpp:1-debian-11

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Installera CMake om specificerat
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# Installera vanliga paket som ofta behövs i skoluppgifter
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    net-tools \
    iputils-ping \
    tcpdump \
    iproute2 \
    vim \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
