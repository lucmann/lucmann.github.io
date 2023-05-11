---
title: Knowledge from "[RFC] Plane color pipeline KMS uAPI"
date: 2023-05-11 15:35:43
tags: [kms]
categories: graphics
---

# Gamma LUT property

Gamma LUT (Look-Up Table) is a property that is used to correct the color and brightness of an image or video. Gamma LUT is basically a table of values that is used to map input values to output values, for example, it may map low input values to higher output values to brighten the image.

Gamma LUT is used to adjust the gamma curve of an image or video, which means it can be used to correct the brightness and contrast of an image. This property is commonly used in image and video processing applications, such as photo editing software, video editing software, and color grading tools.

Gamma LUT can also be used to correct color shifts caused by different lighting conditions or camera settings. By adjusting the gamma curve of an image or video, it is possible to create a more natural and accurate representation of the original scene.

<!--more-->

# CSC property

CSC is an acronym for **Color Space Conversion**, which is a property used in digital image and video processing. CSC is used to convert the color space of an image or video from one format to another.

Color spaces are a way to represent colors using numerical values. Different devices and software applications use different color spaces, which can cause issues when trying to display or process images or videos across different platforms. For example, an image captured by a camera may use one color space, while a monitor may use another color space.

CSC property can be used to convert the color space of an image or video from one format to another. This can be useful for ensuring that images or videos are displayed correctly on different devices or platforms. For example, a video may need to be converted from the color space used by a camera to the color space used by a television or monitor.

CSC can also be used for color correction, where the colors of an image or video are adjusted to make them more accurate or aesthetically pleasing. Overall, the CSC property is an important tool in digital image and video processing, allowing for the correct and accurate representation of colors across different platforms.

# scRGB

scRGB (standardized RGB) is a color space that defines an alternative method for representing colors in digital images or videos. It is a wide-gamut (广色域) color space that can represent a larger range of colors than the traditional sRGB color space used in most computer displays.

scRGB was developed by Microsoft and is defined by the International Electrotechnical Commission (IEC) standard IEC 61966-2-2. It is designed to be a device-independent color space, meaning that it can be used to represent colors accurately on a wide range of devices, including computer monitors, printers, and projectors.

The scRGB color space is based on a linear color system, which means that the difference between two colors is proportional to the difference in their numerical values. This makes it easier to perform accurate color calculations and color corrections in digital image and video processing.

scRGB is commonly used in professional video and film production, as well as in high-end graphics applications. It is also used as a reference color space for other color spaces, such as Adobe RGB and DCI-P3.

## what's the difference between sRGB and scRGB

The main difference between scRGB and sRGB is the range of colors that they can represent. scRGB is a wide-gamut color space that can represent a larger range of colors than sRGB. sRGB is a standard color space that is commonly used in computer displays, but it has a smaller gamut than scRGB.

sRGB defines a color space that can represent approximately 16.8 million colors, while scRGB can represent over a billion colors. This means that scRGB can represent a wider range of colors than sRGB, including more vibrant and saturated colors. However, not all devices or applications can display or process scRGB colors correctly, so it is not always practical to use scRGB in all situations.

Another difference between scRGB and sRGB is their color encoding. scRGB uses a linear color encoding, which means that the difference between two colors is proportional to the difference in their numerical values. sRGB, on the other hand, uses a non-linear encoding, which means that the difference between two colors is not proportional to their numerical values. This makes it easier to display sRGB colors on typical computer monitors and other devices.

Overall, scRGB is a more advanced and flexible color space than sRGB, but it requires more processing power and is not as widely supported. sRGB, on the other hand, is a standard color space that is widely supported by most devices and applications.

# Degamma

Degamma is a process used in digital image and video processing to reverse the gamma correction that is applied to images or videos during encoding. Gamma correction is a non-linear operation that is used to compensate for the non-linear response of human vision to changes in brightness.

Degamma is the process of undoing this non-linear operation to restore the original linear image or video data. This is done to enable further processing or analysis of the image or video data in a linear color space.

During encoding, gamma correction is applied to the image or video data in order to make it easier to display on devices with non-linear displays, such as computer monitors, televisions, and projectors. However, this gamma correction can make it more difficult to perform accurate color grading or other image processing tasks.

Degamma is typically performed by applying an inverse gamma curve to the encoded image or video data. This inverse curve is used to undo the effects of the gamma correction and restore the original linear data. Once the data has been degammaed, it can be processed or analyzed in a linear color space, enabling more accurate color grading and other image processing operations.

# PQ

PQ stands for Perceptual Quantizer (感知量化), which is a type of electro-optical transfer function (电光转换函数) used in high dynamic range (HDR) displays. HDR displays are capable of displaying a wider range of brightness and color than standard dynamic range (SDR) displays, and the PQ transfer function is designed to enable this increased range.

The PQ transfer function is based on human perception of brightness, and it is designed to be more perceptually uniform than traditional gamma curves used in SDR displays. This means that the PQ curve better matches the way that humans perceive changes in brightness, making it easier to display HDR content that looks natural and realistic.

The PQ curve is defined by the International Electrotechnical Commission (IEC) standard IEC 2100-1, and it is used in several HDR standards, including Dolby Vision and HDR10+. The PQ curve maps a range of input values to a range of output values, allowing HDR displays to accurately represent a wide range of brightness and color.

Overall, the PQ transfer function enables HDR displays to deliver a more immersive and realistic viewing experience, with brighter highlights, deeper shadows, and more vibrant colors than SDR displays.

# Android HWC

HWC stands for Hardware Composer. It is a component of the Android operating system that manages the rendering of graphical elements on the screen. It is responsible for taking the output of the various graphical layers in the system, such as the user interface, apps, and games, and compositing them into a single image to be displayed on the screen. The HWC works in conjunction with the graphics hardware on the device to ensure efficient and seamless rendering of the graphical elements. It plays a crucial role in providing a smooth and responsive user experience on Android devices.

## Why does HWC represent hardware composer rather than hardware compositor

The term "composer" is used in reference to HWC because it is responsible for composing, or combining, the various graphical layers into a single image that can be displayed on the screen. The term "compositor" could also be used to describe this function, but "composer" is the term that has been traditionally used in the field of computer graphics to describe this type of operation. Additionally, the term "composer" may be seen as more descriptive of the creative process of combining visual elements, while "compositor" may be seen as more technical in nature. Ultimately, the choice of terminology is a matter of convention and personal preference.
