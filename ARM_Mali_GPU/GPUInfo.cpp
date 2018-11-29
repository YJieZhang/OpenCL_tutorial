//
// Created by ZYJ on 18-11-29.
//
#include <CL/cl.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>

void print(const std::string info, const char* description, const size_t length) {
    std::cout << "[" << info << "]: ";
    for (int i = 0; i < length; ++i) std:: cout << description[i];
    std::cout << std::endl;
}

void printPlatformInfo(cl_platform_id& platform) {
    const size_t LENGTH = 500;
    char description[LENGTH];
    size_t length;
    cl_int error = CL_SUCCESS;

    error = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, LENGTH, description, &length);
    if (error == CL_SUCCESS)
        print("CL_PLATFORM_PROFILE   ", description, length);

    error = clGetPlatformInfo(platform, CL_PLATFORM_VERSION, LENGTH, description, &length);
    if (error == CL_SUCCESS)
        print("CL_PLATFORM_VERSION   ", description, length);

    error = clGetPlatformInfo(platform, CL_PLATFORM_NAME, LENGTH, description, &length);
    if (error == CL_SUCCESS)
        print("CL_PLATFORM_NAME      ", description, length);

    error = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, LENGTH, description, &length);
    if (error == CL_SUCCESS)
        print("CL_PLATFORM_VENDOR    ", description, length);

    error = clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS, LENGTH, description, &length);
    if (error == CL_SUCCESS)
        print("CL_PLATFORM_EXTENSIONS", description, length);
}

std::vector<cl_platform_id> getPlatforms() {
    cl_uint platforms_count;
    cl_int error = CL_SUCCESS;
    clGetPlatformIDs(0, nullptr, &platforms_count);
    std::vector<cl_platform_id> platforms(platforms_count);
    clGetPlatformIDs(platforms_count, platforms.data(), nullptr);
    std::cout << "Number of platforms: " << platforms_count << ".\n";

    for (auto& platform : platforms) {
        printPlatformInfo(platform);
    }
    std::cout << std::endl;
    return platforms;
}

void print(const std::string info, const cl_int num) {
    std::cout << "[" << info << "]: " << num << std::endl;
}

void print(const std::string info, const cl_uint num) {
    std::cout << "[" << info << "]: " << num << std::endl;
}

void print(const std::string info, const cl_ulong num) {
    std::cout << "[" << info << "]: " << num << std::endl;
}

void print(const std::string info, const std::string type) {
    std::cout << "[" << info << "]: " << type << std::endl;
}

void printGPUInfo(cl_device_id& gpu) {
    const size_t LENGTH = 500;
    char description[LENGTH];
    size_t length;
    cl_int error = CL_SUCCESS;

    cl_uint return_cl_uint;
    error = clGetDeviceInfo(gpu, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(return_cl_uint), &return_cl_uint, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_MAX_COMPUTE_UNITS", return_cl_uint);
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(return_cl_uint), &return_cl_uint, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS", return_cl_uint);
    }

    size_t return_size_t;
    error = clGetDeviceInfo(gpu, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(return_size_t), &return_size_t, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_MAX_WORK_GROUP_SIZE", return_size_t);
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(return_cl_uint), &return_cl_uint, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT", return_cl_uint);
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, sizeof(return_cl_uint), &return_cl_uint, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT", return_cl_uint);
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(return_cl_uint), &return_cl_uint, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_MAX_CLOCK_FREQUENCY", return_cl_uint);
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_ADDRESS_BITS, sizeof(return_cl_uint), &return_cl_uint, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_ADDRESS_BITS", return_cl_uint);
    }

    cl_ulong return_cl_ulong;
    error = clGetDeviceInfo(gpu, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(return_cl_ulong), &return_cl_ulong, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_MAX_MEM_ALLOC_SIZE", return_cl_ulong);
    }

    cl_bool return_cl_bool;
    error = clGetDeviceInfo(gpu, CL_DEVICE_IMAGE_SUPPORT, sizeof(return_cl_bool), &return_cl_bool, nullptr);
    if (error == CL_SUCCESS) {
        if (return_cl_bool) {
            print("CL_DEVICE_IMAGE_SUPPORT", 1);
        } else {
            print("CL_DEVICE_IMAGE_SUPPORT", 0);
        }
    }

    cl_device_mem_cache_type return_cache_type;
    error = clGetDeviceInfo(gpu, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(return_cache_type), &return_cache_type, nullptr);
    if (error == CL_SUCCESS) {
        if (return_cache_type == CL_NONE) {
            print("CL_DEVICE_GLOBAL_MEM_CACHE_TYPE", "CL_NONE");
        } else if (return_cache_type == CL_READ_ONLY_CACHE) {
            print("CL_DEVICE_GLOBAL_MEM_CACHE_TYPE", "CL_READ_ONLY_CACHE");
        } else {
            print("CL_DEVICE_GLOBAL_MEM_CACHE_TYPE", "CL_READ_WRITE_CACHE");
        }
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(return_cl_ulong), &return_cl_ulong, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_GLOBAL_MEM_SIZE", return_cl_ulong);
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(return_cl_ulong), &return_cl_ulong, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_GLOBAL_MEM_CACHE_SIZE", return_cl_ulong);
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(return_cl_uint), &return_cl_uint, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE", return_cl_uint);
    }

    cl_device_local_mem_type return_mem_type;
    error = clGetDeviceInfo(gpu, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(return_mem_type), &return_mem_type, nullptr);
    if (error == CL_SUCCESS) {
        if (return_mem_type == CL_LOCAL) {
            print("CL_DEVICE_LOCAL_MEM_TYPE", "CL_LOCAL");
        } else {
            print("CL_DEVICE_LOCAL_MEM_TYPE", "CL_GLOBAL");
        }
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(return_cl_ulong), &return_cl_ulong, nullptr);
    if (error == CL_SUCCESS) {
        print("CL_DEVICE_LOCAL_MEM_SIZE", return_cl_ulong);
    }

    error = clGetDeviceInfo(gpu, CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(return_cl_bool), &return_cl_bool, nullptr);
    if (error == CL_SUCCESS) {
        if (return_cl_bool) {
            print("CL_DEVICE_HOST_UNIFIED_MEMORY", 1);
        } else {
            print("CL_DEVICE_HOST_UNIFIED_MEMORY", 0);
        }
    }
}

std::vector<cl_device_id> getGPUs(cl_platform_id& platform) {
    cl_uint devices_count;
    cl_int error = CL_SUCCESS;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &devices_count);
    std::vector<cl_device_id> devices(devices_count);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, devices_count, devices.data(), nullptr);
    std::cout << "Number of devices(GPU): " << devices_count << ".\n";

    for (auto& gpu : devices) {
        printGPUInfo(gpu);
    }
    std::cout << std::endl;
    return devices;
}

int main(int argc, char* argv[]) {
    auto platforms = getPlatforms();
    auto gpus = getGPUs(platforms[0]);
    return 0;
}