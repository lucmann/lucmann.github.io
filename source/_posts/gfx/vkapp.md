---
title: Vulkan Application
date: 2024-09-18 21:21:27
tags: vulkan
categories: graphics
---

Vulkan 是一个低开销、跨平台的二维和三维图形与计算的应用程序接口，由 Khronos 在2015年在 GDC 上首次发布。它旨在提供高效能和更均衡的 CPU 和 GPU 占用，类似于 Direct3D 12 和 AMD Mantle。

本文主要记录一下在 llvmpipe 软渲染 Vulkan 驱动下， 一个 vulkan 应用程序 ([demo](https://github.com/lucmann/VulkanExamples/tree/build-for-linux/examples/deferredmultisampling)) 的执行过程。

<!--more-->

# Call Stack

```
libvulkan_lvp.so!pipe_loader_sw_create_screen(struct pipe_loader_device * dev, const struct pipe_screen_config * config, _Bool sw_vk) (\home\luc\gh\forked\src\gallium\auxiliary\pipe-loader\pipe_loader_sw.c:427)
libvulkan_lvp.so!pipe_loader_create_screen_vk(struct pipe_loader_device * dev, _Bool sw_vk, _Bool driver_name_is_inferred) (\home\luc\gh\forked\src\gallium\auxiliary\pipe-loader\pipe_loader.c:181)
libvulkan_lvp.so!lvp_physical_device_init(struct lvp_physical_device * device, struct lvp_instance * instance, struct pipe_loader_device * pld) (\home\luc\gh\forked\src\gallium\frontends\lavapipe\lvp_device.c:1240)
libvulkan_lvp.so!lvp_enumerate_physical_devices(struct vk_instance * vk_instance) (\home\luc\gh\forked\src\gallium\frontends\lavapipe\lvp_device.c:1416)
libvulkan_lvp.so!enumerate_physical_devices_locked(struct vk_instance * instance) (\home\luc\gh\forked\src\vulkan\runtime\vk_instance.c:434)
libvulkan_lvp.so!enumerate_physical_devices(struct vk_instance * instance) (\home\luc\gh\forked\src\vulkan\runtime\vk_instance.c:459)
libvulkan_lvp.so!vk_common_EnumeratePhysicalDevices(VkInstance _instance, uint32_t * pPhysicalDeviceCount, VkPhysicalDevice * pPhysicalDevices) (\home\luc\gh\forked\src\vulkan\runtime\vk_instance.c:475)
libvulkan.so.1![Unknown/Just-In-Time compiled code] (Unknown Source:0)
libvulkan.so.1!vkEnumeratePhysicalDevices (Unknown Source:0)
vk::DispatchLoaderStatic::vkEnumeratePhysicalDevices(const vk::DispatchLoaderStatic * const this, VkInstance instance, uint32_t * pPhysicalDeviceCount, VkPhysicalDevice * pPhysicalDevices) (\usr\local\include\vulkan\vulkan.hpp:1047)
vk::Instance::enumeratePhysicalDevices<std::allocator<vk::PhysicalDevice>, vk::DispatchLoaderStatic>(const vk::DispatchLoaderStatic & d, const vk::Instance * const this) (\usr\local\include\vulkan\vulkan_funcs.hpp:120)
vks::Context::pickDevice(vks::Context * const this, const vk::SurfaceKHR & surface) (\home\luc\gh\VulkanExamples\base\vks\context.hpp:426)
vks::Context::createDevice(vks::Context * const this, const vk::SurfaceKHR & surface) (\home\luc\gh\VulkanExamples\base\vks\context.hpp:233)
vkx::ExampleBase::initVulkan(vkx::ExampleBase * const this) (\home\luc\gh\VulkanExamples\base\vulkanExampleBase.cpp:123)
vkx::ExampleBase::run(vkx::ExampleBase * const this) (\home\luc\gh\VulkanExamples\base\vulkanExampleBase.cpp:75)
main(const int argc, const char ** argv) (\home\luc\gh\VulkanExamples\examples\deferredmultisampling\deferredmultisampling.cpp:684)
```
