# SPDX-License-Identifier: LGPL-2.1-or-later
# This Dockerfile is used to build FreeCAD using Ubuntu 24.04 LTS and conda.

# Base image
FROM ubuntu:24.04

# Set the maintainer label
LABEL maintainer="FreeCAD Maintainer <maintainer@freecad.org>"

# Set the working directory to /workspace, which will also be mounted to the current directory on host
WORKDIR /workspace

# Install system dependencies
RUN echo $'\n\
Types: deb \n\
URIs: http://archive.ubuntu.com/ubuntu/ \n\
Suites: oracular \n\
Components: main universe restricted multiverse \n\
Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg \n' \
>> /etc/apt/sources.list.d/ubuntu.sources && \
    apt-get update -qq && \
    apt-get install -y --no-install-recommends \
    wget \
    curl \
    cmake \
    sudo \
    git \
    build-essential \
    freeglut3-dev \
    libcoin-dev \
    libegl1 \
    libgl1 \
    libglx0 \
    libmedc-dev \
    libocct*-dev \
    libopengl0 \
    libpyside6-dev \
    libshiboken6-dev \
    libvtk9-dev \
    mesa-utils \
    ninja-build \
    pyside6-tools \
    python3-pivy \
    qt6-base-dev \
    qt6-svg-dev \
    qt6-tools-dev \
    swig \
    xvfb \
        libxerces-c-dev \
        libyaml-cpp-dev \
        libzipios++-dev \
        libboost-dev \
        libboost-date-time-dev \
        libboost-filesystem-dev \
        libboost-graph-dev \
        libboost-iostreams-dev \
        libboost-program-options-dev \
        libboost-python-dev \
        libboost-regex-dev \
        libboost-serialization-dev \
        libboost-thread-dev \
    ca-certificates && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* \
    mkdir /usr/lib/include/ \
    ln -s /usr/include/PySide6 /usr/lib/include/PySide6

# Download and install Miniconda based on the architecture
RUN ARCH=$(uname -m) && \
    if [ "$ARCH" = "x86_64" ]; then \
        MINICONDA_URL="https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh"; \
    elif [ "$ARCH" = "aarch64" ]; then \
        MINICONDA_URL="https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-aarch64.sh"; \
    else \
        echo "Unsupported architecture: $ARCH"; exit 1; \
    fi && \
    curl -fsSL --retry 5 $MINICONDA_URL -o /tmp/miniconda.sh && \
    bash /tmp/miniconda.sh -b -p /opt/miniconda && \
    rm /tmp/miniconda.sh
    #&& \
    #/opt/miniconda/bin/conda init

# Update PATH to include conda
#ENV PATH="/opt/miniconda/bin:$PATH"

# Install mamba for faster package management
#RUN conda install -n base -c conda-forge mamba -y
#RUN conda install -n freecad xerces-c -y  # installed using apt instead
#RUN conda install -n base -c conda-forge libmed -y

# Temporarily copy the conda environment files into the image for environment setup
COPY conda/ /workspace/conda/

# Create and update the conda environment using mamba with retries
# RUN for i in 1 2 3; do \
#         mamba env create -f /workspace/conda/conda-env-qt6.yaml && break || \
#         (echo "Retrying mamba environment creation ($i/3)" && mamba clean --all -f); \
#     done

# Set environment variables for FreeCAD build
ENV CONDA_ENV_PATH="/opt/miniconda/envs/freecad"
#ENV QT_HOST_PATH="$CONDA_ENV_PATH"

ENV BUILDDIR="/workspace/build/release"

# Create necessary directories
RUN mkdir -p ${BUILDDIR}

# Execute the environment setup script
#RUN bash /workspace/conda/setup-environment-qt6.sh

CMD ["/bin/bash"]