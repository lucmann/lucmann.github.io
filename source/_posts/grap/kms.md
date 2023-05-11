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
