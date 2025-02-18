---
title: Linux Power Management
date: 2025-02-18 11:38:10
tags: [linux]
categories: linux
---

# Power State

| Level | State           | Power            | Command<br>(systemctl) |
|:------|:----------------|:-----------------|:-----------------------|
| S0    | idle            | full             | N/A                    |
| S1    | sleep           | low              | N/A                    |
| S2    | deeper sleep    | lower            | N/A                    |
| S3    | suspend to RAM  | CPU off RAM on   | suspend                |
| S4    | suspend to DISK | Most devices off | hibernate              |
| S5    | shutdown        | completely off   | poweroff               |

<!--more-->
