=============================
User Guide for AMDGPU Backend
=============================

.. contents::
   :local:

.. toctree::
   :hidden:

   AMDGPU/AMDGPUAsmGFX7
   AMDGPU/AMDGPUAsmGFX8
   AMDGPU/AMDGPUAsmGFX9
   AMDGPU/AMDGPUAsmGFX900
   AMDGPU/AMDGPUAsmGFX904
   AMDGPU/AMDGPUAsmGFX906
   AMDGPU/AMDGPUAsmGFX908
   AMDGPU/AMDGPUAsmGFX90a
   AMDGPU/AMDGPUAsmGFX940
   AMDGPU/AMDGPUAsmGFX10
   AMDGPU/AMDGPUAsmGFX1011
   AMDGPU/AMDGPUAsmGFX1013
   AMDGPU/AMDGPUAsmGFX1030
   AMDGPU/AMDGPUAsmGFX11
   AMDGPUModifierSyntax
   AMDGPUOperandSyntax
   AMDGPUInstructionSyntax
   AMDGPUInstructionNotation
   AMDGPUDwarfExtensionsForHeterogeneousDebugging
   AMDGPUDwarfExtensionAllowLocationDescriptionOnTheDwarfExpressionStack/AMDGPUDwarfExtensionAllowLocationDescriptionOnTheDwarfExpressionStack

Introduction
============

The AMDGPU backend provides ISA code generation for AMD GPUs, starting with the
R600 family up until the current GCN families. It lives in the
``llvm/lib/Target/AMDGPU`` directory.

LLVM
====

.. _amdgpu-target-triples:

Target Triples
--------------

Use the Clang option ``-target <Architecture>-<Vendor>-<OS>-<Environment>``
to specify the target triple:

  .. table:: AMDGPU Architectures
     :name: amdgpu-architecture-table

     ============ ==============================================================
     Architecture Description
     ============ ==============================================================
     ``r600``     AMD GPUs HD2XXX-HD6XXX for graphics and compute shaders.
     ``amdgcn``   AMD GPUs GCN GFX6 onwards for graphics and compute shaders.
     ============ ==============================================================

  .. table:: AMDGPU Vendors
     :name: amdgpu-vendor-table

     ============ ==============================================================
     Vendor       Description
     ============ ==============================================================
     ``amd``      Can be used for all AMD GPU usage.
     ``mesa3d``   Can be used if the OS is ``mesa3d``.
     ============ ==============================================================

  .. table:: AMDGPU Operating Systems
     :name: amdgpu-os

     ============== ============================================================
     OS             Description
     ============== ============================================================
     *<empty>*      Defaults to the *unknown* OS.
     ``amdhsa``     Compute kernels executed on HSA [HSA]_ compatible runtimes
                    such as:

                    - AMD's ROCm™ runtime [AMD-ROCm]_ using the *rocm-amdhsa*
                      loader on Linux. See *AMD ROCm Platform Release Notes*
                      [AMD-ROCm-Release-Notes]_ for supported hardware and
                      software.
                    - AMD's PAL runtime using the *pal-amdhsa* loader on
                      Windows.

     ``amdpal``     Graphic shaders and compute kernels executed on AMD's PAL
                    runtime using the *pal-amdpal* loader on Windows and Linux
                    Pro.
     ``mesa3d``     Graphic shaders and compute kernels executed on AMD's Mesa
                    3D runtime using the *mesa-mesa3d* loader on Linux.
     ============== ============================================================

  .. table:: AMDGPU Environments
     :name: amdgpu-environment-table

     ============ ==============================================================
     Environment  Description
     ============ ==============================================================
     *<empty>*    Default.
     ============ ==============================================================

.. _amdgpu-processors:

Processors
----------

Use the Clang options ``-mcpu=<target-id>`` or ``--offload-arch=<target-id>`` to
specify the AMDGPU processor together with optional target features. See
:ref:`amdgpu-target-id` and :ref:`amdgpu-target-features` for AMD GPU target
specific information.

Every processor supports every OS ABI (see :ref:`amdgpu-os`) with the following exceptions:

* ``amdhsa`` is not supported in ``r600`` architecture (see :ref:`amdgpu-architecture-table`).


  .. table:: AMDGPU Processors
     :name: amdgpu-processor-table

     =========== =============== ============ ===== ================= =============== =============== ======================
     Processor   Alternative     Target       dGPU/ Target            Target          OS Support      Example
                 Processor       Triple       APU   Features          Properties      *(see*          Products
                                 Architecture       Supported                         `amdgpu-os`_
                                                                                      *and
                                                                                      corresponding
                                                                                      runtime release
                                                                                      notes for
                                                                                      current
                                                                                      information and
                                                                                      level of
                                                                                      support)*
     =========== =============== ============ ===== ================= =============== =============== ======================
     **Radeon HD 2000/3000 Series (R600)** [AMD-RADEON-HD-2000-3000]_
     -----------------------------------------------------------------------------------------------------------------------
     ``r600``                    ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``r630``                    ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``rs880``                   ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``rv670``                   ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     **Radeon HD 4000 Series (R700)** [AMD-RADEON-HD-4000]_
     -----------------------------------------------------------------------------------------------------------------------
     ``rv710``                   ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``rv730``                   ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``rv770``                   ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     **Radeon HD 5000 Series (Evergreen)** [AMD-RADEON-HD-5000]_
     -----------------------------------------------------------------------------------------------------------------------
     ``cedar``                   ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``cypress``                 ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``juniper``                 ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``redwood``                 ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``sumo``                    ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     **Radeon HD 6000 Series (Northern Islands)** [AMD-RADEON-HD-6000]_
     -----------------------------------------------------------------------------------------------------------------------
     ``barts``                   ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``caicos``                  ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``cayman``                  ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``turks``                   ``r600``     dGPU                    - Does not
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     **GCN GFX6 (Southern Islands (SI))** [AMD-GCN-GFX6]_
     -----------------------------------------------------------------------------------------------------------------------
     ``gfx600``  - ``tahiti``    ``amdgcn``   dGPU                    - Does not      - *pal-amdpal*
                                                                        support
                                                                        generic
                                                                        address
                                                                        space
     ``gfx601``  - ``pitcairn``  ``amdgcn``   dGPU                    - Does not      - *pal-amdpal*
                 - ``verde``                                            support
                                                                        generic
                                                                        address
                                                                        space
     ``gfx602``  - ``hainan``    ``amdgcn``   dGPU                    - Does not      - *pal-amdpal*
                 - ``oland``                                            support
                                                                        generic
                                                                        address
                                                                        space
     **GCN GFX7 (Sea Islands (CI))** [AMD-GCN-GFX7]_
     -----------------------------------------------------------------------------------------------------------------------
     ``gfx700``  - ``kaveri``    ``amdgcn``   APU                     - Offset        - *rocm-amdhsa* - A6-7000
                                                                        flat          - *pal-amdhsa*  - A6 Pro-7050B
                                                                        scratch       - *pal-amdpal*  - A8-7100
                                                                                                      - A8 Pro-7150B
                                                                                                      - A10-7300
                                                                                                      - A10 Pro-7350B
                                                                                                      - FX-7500
                                                                                                      - A8-7200P
                                                                                                      - A10-7400P
                                                                                                      - FX-7600P
     ``gfx701``  - ``hawaii``    ``amdgcn``   dGPU                    - Offset        - *rocm-amdhsa* - FirePro W8100
                                                                        flat          - *pal-amdhsa*  - FirePro W9100
                                                                        scratch       - *pal-amdpal*  - FirePro S9150
                                                                                                      - FirePro S9170
     ``gfx702``                  ``amdgcn``   dGPU                    - Offset        - *rocm-amdhsa* - Radeon R9 290
                                                                        flat          - *pal-amdhsa*  - Radeon R9 290x
                                                                        scratch       - *pal-amdpal*  - Radeon R390
                                                                                                      - Radeon R390x
     ``gfx703``  - ``kabini``    ``amdgcn``   APU                     - Offset        - *pal-amdhsa*  - E1-2100
                 - ``mullins``                                          flat          - *pal-amdpal*  - E1-2200
                                                                        scratch                       - E1-2500
                                                                                                      - E2-3000
                                                                                                      - E2-3800
                                                                                                      - A4-5000
                                                                                                      - A4-5100
                                                                                                      - A6-5200
                                                                                                      - A4 Pro-3340B
     ``gfx704``  - ``bonaire``   ``amdgcn``   dGPU                    - Offset        - *pal-amdhsa*  - Radeon HD 7790
                                                                        flat          - *pal-amdpal*  - Radeon HD 8770
                                                                        scratch                       - R7 260
                                                                                                      - R7 260X
     ``gfx705``                  ``amdgcn``   APU                     - Offset        - *pal-amdhsa*  *TBA*
                                                                        flat          - *pal-amdpal*
                                                                        scratch                       .. TODO::

                                                                                                        Add product
                                                                                                        names.

     **GCN GFX8 (Volcanic Islands (VI))** [AMD-GCN-GFX8]_
     -----------------------------------------------------------------------------------------------------------------------
     ``gfx801``  - ``carrizo``   ``amdgcn``   APU   - xnack           - Offset        - *rocm-amdhsa* - A6-8500P
                                                                        flat          - *pal-amdhsa*  - Pro A6-8500B
                                                                        scratch       - *pal-amdpal*  - A8-8600P
                                                                                                      - Pro A8-8600B
                                                                                                      - FX-8800P
                                                                                                      - Pro A12-8800B
                                                                                                      - A10-8700P
                                                                                                      - Pro A10-8700B
                                                                                                      - A10-8780P
                                                                                                      - A10-9600P
                                                                                                      - A10-9630P
                                                                                                      - A12-9700P
                                                                                                      - A12-9730P
                                                                                                      - FX-9800P
                                                                                                      - FX-9830P
                                                                                                      - E2-9010
                                                                                                      - A6-9210
                                                                                                      - A9-9410
     ``gfx802``  - ``iceland``   ``amdgcn``   dGPU                    - Offset        - *rocm-amdhsa* - Radeon R9 285
                 - ``tonga``                                            flat          - *pal-amdhsa*  - Radeon R9 380
                                                                        scratch       - *pal-amdpal*  - Radeon R9 385
     ``gfx803``  - ``fiji``      ``amdgcn``   dGPU                                    - *rocm-amdhsa* - Radeon R9 Nano
                                                                                      - *pal-amdhsa*  - Radeon R9 Fury
                                                                                      - *pal-amdpal*  - Radeon R9 FuryX
                                                                                                      - Radeon Pro Duo
                                                                                                      - FirePro S9300x2
                                                                                                      - Radeon Instinct MI8
     \           - ``polaris10`` ``amdgcn``   dGPU                    - Offset        - *rocm-amdhsa* - Radeon RX 470
                                                                        flat          - *pal-amdhsa*  - Radeon RX 480
                                                                        scratch       - *pal-amdpal*  - Radeon Instinct MI6
     \           - ``polaris11`` ``amdgcn``   dGPU                    - Offset        - *rocm-amdhsa* - Radeon RX 460
                                                                        flat          - *pal-amdhsa*
                                                                        scratch       - *pal-amdpal*
     ``gfx805``  - ``tongapro``  ``amdgcn``   dGPU                    - Offset        - *rocm-amdhsa* - FirePro S7150
                                                                        flat          - *pal-amdhsa*  - FirePro S7100
                                                                        scratch       - *pal-amdpal*  - FirePro W7100
                                                                                                      - Mobile FirePro
                                                                                                        M7170
     ``gfx810``  - ``stoney``    ``amdgcn``   APU   - xnack           - Offset        - *rocm-amdhsa* *TBA*
                                                                        flat          - *pal-amdhsa*
                                                                        scratch       - *pal-amdpal*  .. TODO::

                                                                                                        Add product
                                                                                                        names.

     **GCN GFX9 (Vega)** [AMD-GCN-GFX900-GFX904-VEGA]_ [AMD-GCN-GFX906-VEGA7NM]_ [AMD-GCN-GFX908-CDNA1]_ [AMD-GCN-GFX90A-CDNA2]_
     -----------------------------------------------------------------------------------------------------------------------
     ``gfx900``                  ``amdgcn``   dGPU  - xnack           - Absolute      - *rocm-amdhsa* - Radeon Vega
                                                                        flat          - *pal-amdhsa*    Frontier Edition
                                                                        scratch       - *pal-amdpal*  - Radeon RX Vega 56
                                                                                                      - Radeon RX Vega 64
                                                                                                      - Radeon RX Vega 64
                                                                                                        Liquid
                                                                                                      - Radeon Instinct MI25
     ``gfx902``                  ``amdgcn``   APU   - xnack           - Absolute      - *rocm-amdhsa* - Ryzen 3 2200G
                                                                        flat          - *pal-amdhsa*  - Ryzen 5 2400G
                                                                        scratch       - *pal-amdpal*
     ``gfx904``                  ``amdgcn``   dGPU  - xnack                           - *rocm-amdhsa* *TBA*
                                                                                      - *pal-amdhsa*
                                                                                      - *pal-amdpal*  .. TODO::

                                                                                                        Add product
                                                                                                        names.

     ``gfx906``                  ``amdgcn``   dGPU  - sramecc         - Absolute      - *rocm-amdhsa* - Radeon Instinct MI50
                                                    - xnack             flat          - *pal-amdhsa*  - Radeon Instinct MI60
                                                                        scratch       - *pal-amdpal*  - Radeon VII
                                                                                                      - Radeon Pro VII
     ``gfx908``                  ``amdgcn``   dGPU  - sramecc                         - *rocm-amdhsa* - AMD Instinct MI100 Accelerator
                                                    - xnack           - Absolute
                                                                        flat
                                                                        scratch
     ``gfx909``                  ``amdgcn``   APU   - xnack           - Absolute      - *pal-amdpal*  *TBA*
                                                                        flat
                                                                        scratch                       .. TODO::

                                                                                                        Add product
                                                                                                        names.

     ``gfx90a``                  ``amdgcn``   dGPU  - sramecc         - Absolute      - *rocm-amdhsa* *TBA*
                                                    - tgsplit           flat
                                                    - xnack             scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     ``gfx90c``                  ``amdgcn``   APU   - xnack           - Absolute      - *pal-amdpal*  - Ryzen 7 4700G
                                                                        flat                          - Ryzen 7 4700GE
                                                                        scratch                       - Ryzen 5 4600G
                                                                                                      - Ryzen 5 4600GE
                                                                                                      - Ryzen 3 4300G
                                                                                                      - Ryzen 3 4300GE
                                                                                                      - Ryzen Pro 4000G
                                                                                                      - Ryzen 7 Pro 4700G
                                                                                                      - Ryzen 7 Pro 4750GE
                                                                                                      - Ryzen 5 Pro 4650G
                                                                                                      - Ryzen 5 Pro 4650GE
                                                                                                      - Ryzen 3 Pro 4350G
                                                                                                      - Ryzen 3 Pro 4350GE

     ``gfx940``                  ``amdgcn``   dGPU  - sramecc         - Architected                   *TBA*
                                                    - tgsplit           flat
                                                    - xnack             scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     ``gfx941``                  ``amdgcn``   dGPU  - sramecc         - Architected                   *TBA*
                                                    - tgsplit           flat
                                                    - xnack             scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     ``gfx942``                  ``amdgcn``   dGPU  - sramecc         - Architected                   *TBA*
                                                    - tgsplit           flat
                                                    - xnack             scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     **GCN GFX10.1 (RDNA 1)** [AMD-GCN-GFX10-RDNA1]_
     -----------------------------------------------------------------------------------------------------------------------
     ``gfx1010``                 ``amdgcn``   dGPU  - cumode          - Absolute      - *rocm-amdhsa* - Radeon RX 5700
                                                    - wavefrontsize64   flat          - *pal-amdhsa*  - Radeon RX 5700 XT
                                                    - xnack             scratch       - *pal-amdpal*  - Radeon Pro 5600 XT
                                                                                                      - Radeon Pro 5600M
     ``gfx1011``                 ``amdgcn``   dGPU  - cumode                          - *rocm-amdhsa* - Radeon Pro V520
                                                    - wavefrontsize64 - Absolute      - *pal-amdhsa*
                                                    - xnack             flat          - *pal-amdpal*
                                                                        scratch
     ``gfx1012``                 ``amdgcn``   dGPU  - cumode          - Absolute      - *rocm-amdhsa* - Radeon RX 5500
                                                    - wavefrontsize64   flat          - *pal-amdhsa*  - Radeon RX 5500 XT
                                                    - xnack             scratch       - *pal-amdpal*
     ``gfx1013``                 ``amdgcn``   APU   - cumode          - Absolute      - *rocm-amdhsa* *TBA*
                                                    - wavefrontsize64   flat          - *pal-amdhsa*
                                                    - xnack             scratch       - *pal-amdpal*  .. TODO::

                                                                                                        Add product
                                                                                                        names.

     **GCN GFX10.3 (RDNA 2)** [AMD-GCN-GFX10-RDNA2]_
     -----------------------------------------------------------------------------------------------------------------------
     ``gfx1030``                 ``amdgcn``   dGPU  - cumode          - Absolute      - *rocm-amdhsa* - Radeon RX 6800
                                                    - wavefrontsize64   flat          - *pal-amdhsa*  - Radeon RX 6800 XT
                                                                        scratch       - *pal-amdpal*  - Radeon RX 6900 XT
     ``gfx1031``                 ``amdgcn``   dGPU  - cumode          - Absolute      - *rocm-amdhsa* - Radeon RX 6700 XT
                                                    - wavefrontsize64   flat          - *pal-amdhsa*
                                                                        scratch       - *pal-amdpal*
     ``gfx1032``                 ``amdgcn``   dGPU  - cumode          - Absolute      - *rocm-amdhsa* *TBA*
                                                    - wavefrontsize64   flat          - *pal-amdhsa*
                                                                        scratch       - *pal-amdpal*  .. TODO::

                                                                                                        Add product
                                                                                                        names.

     ``gfx1033``                 ``amdgcn``   APU   - cumode          - Absolute      - *pal-amdpal*  *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::

                                                                                                        Add product
                                                                                                        names.
     ``gfx1034``                 ``amdgcn``   dGPU  - cumode          - Absolute      - *pal-amdpal*  *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::

                                                                                                        Add product
                                                                                                        names.

     ``gfx1035``                 ``amdgcn``   APU   - cumode          - Absolute      - *pal-amdpal*  *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::
                                                                                                        Add product
                                                                                                        names.

     ``gfx1036``                 ``amdgcn``   APU   - cumode          - Absolute      - *pal-amdpal*  *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::

                                                                                                        Add product
                                                                                                        names.

     **GCN GFX11 (RDNA 3)** [AMD-GCN-GFX11-RDNA3]_
     -----------------------------------------------------------------------------------------------------------------------
     ``gfx1100``                 ``amdgcn``   dGPU  - cumode          - Architected   - *pal-amdpal*  *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     ``gfx1101``                 ``amdgcn``   dGPU  - cumode          - Architected                   *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     ``gfx1102``                 ``amdgcn``   dGPU  - cumode          - Architected                   *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     ``gfx1103``                 ``amdgcn``   APU   - cumode          - Architected                   *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     ``gfx1150``                 ``amdgcn``   APU   - cumode          - Architected                   *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     ``gfx1151``                 ``amdgcn``   APU   - cumode          - Architected                   *TBA*
                                                    - wavefrontsize64   flat
                                                                        scratch                       .. TODO::
                                                                      - Packed
                                                                        work-item                       Add product
                                                                        IDs                             names.

     =========== =============== ============ ===== ================= =============== =============== ======================

.. _amdgpu-target-features:

Target Features
---------------

Target features control how code is generated to support certain
processor specific features. Not all target features are supported by
all processors. The runtime must ensure that the features supported by
the device used to execute the code match the features enabled when
generating the code. A mismatch of features may result in incorrect
execution, or a reduction in performance.

The target features supported by each processor is listed in
:ref:`amdgpu-processor-table`.

Target features are controlled by exactly one of the following Clang
options:

``-mcpu=<target-id>`` or ``--offload-arch=<target-id>``

  The ``-mcpu`` and ``--offload-arch`` can specify the target feature as
  optional components of the target ID. If omitted, the target feature has the
  ``any`` value. See :ref:`amdgpu-target-id`.

``-m[no-]<target-feature>``

  Target features not specified by the target ID are specified using a
  separate option. These target features can have an ``on`` or ``off``
  value.  ``on`` is specified by omitting the ``no-`` prefix, and
  ``off`` is specified by including the ``no-`` prefix. The default
  if not specified is ``off``.

For example:

``-mcpu=gfx908:xnack+``
  Enable the ``xnack`` feature.
``-mcpu=gfx908:xnack-``
  Disable the ``xnack`` feature.
``-mcumode``
  Enable the ``cumode`` feature.
``-mno-cumode``
  Disable the ``cumode`` feature.

  .. table:: AMDGPU Target Features
     :name: amdgpu-target-features-table

     =============== ============================ ==================================================
     Target Feature  Clang Option to Control      Description
     Name
     =============== ============================ ==================================================
     cumode          - ``-m[no-]cumode``          Control the wavefront execution mode used
                                                  when generating code for kernels. When disabled
                                                  native WGP wavefront execution mode is used,
                                                  when enabled CU wavefront execution mode is used
                                                  (see :ref:`amdgpu-amdhsa-memory-model`).

     sramecc         - ``-mcpu``                  If specified, generate code that can only be
                     - ``--offload-arch``         loaded and executed in a process that has a
                                                  matching setting for SRAMECC.

                                                  If not specified for code object V2 to V3, generate
                                                  code that can be loaded and executed in a process
                                                  with SRAMECC enabled.

                                                  If not specified for code object V4 or above, generate
                                                  code that can be loaded and executed in a process
                                                  with either setting of SRAMECC.

     tgsplit           ``-m[no-]tgsplit``         Enable/disable generating code that assumes
                                                  work-groups are launched in threadgroup split mode.
                                                  When enabled the waves of a work-group may be
                                                  launched in different CUs.

     wavefrontsize64 - ``-m[no-]wavefrontsize64`` Control the wavefront size used when
                                                  generating code for kernels. When disabled
                                                  native wavefront size 32 is used, when enabled
                                                  wavefront size 64 is used.

     xnack           - ``-mcpu``                  If specified, generate code that can only be
                     - ``--offload-arch``         loaded and executed in a process that has a
                                                  matching setting for XNACK replay.

                                                  If not specified for code object V2 to V3, generate
                                                  code that can be loaded and executed in a process
                                                  with XNACK replay enabled.

                                                  If not specified for code object V4 or above, generate
                                                  code that can be loaded and executed in a process
                                                  with either setting of XNACK replay.

                                                  XNACK replay can be used for demand paging and
                                                  page migration. If enabled in the device, then if
                                                  a page fault occurs the code may execute
                                                  incorrectly unless generated with XNACK replay
                                                  enabled, or generated for code object V4 or above without
                                                  specifying XNACK replay. Executing code that was
                                                  generated with XNACK replay enabled, or generated
                                                  for code object V4 or above without specifying XNACK replay,
                                                  on a device that does not have XNACK replay
                                                  enabled will execute correctly but may be less
                                                  performant than code generated for XNACK replay
                                                  disabled.
     =============== ============================ ==================================================

.. _amdgpu-target-id:

Target ID
---------

AMDGPU supports target IDs. See `Clang Offload Bundler
<https://clang.llvm.org/docs/ClangOffloadBundler.html>`_ for a general
description. The AMDGPU target specific information is:

**processor**
  Is an AMDGPU processor or alternative processor name specified in
  :ref:`amdgpu-processor-table`. The non-canonical form target ID allows both
  the primary processor and alternative processor names. The canonical form
  target ID only allow the primary processor name.

**target-feature**
  Is a target feature name specified in :ref:`amdgpu-target-features-table` that
  is supported by the processor. The target features supported by each processor
  is specified in :ref:`amdgpu-processor-table`. Those that can be specified in
  a target ID are marked as being controlled by ``-mcpu`` and
  ``--offload-arch``. Each target feature must appear at most once in a target
  ID. The non-canonical form target ID allows the target features to be
  specified in any order. The canonical form target ID requires the target
  features to be specified in alphabetic order.

.. _amdgpu-target-id-v2-v3:

Code Object V2 to V3 Target ID
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The target ID syntax for code object V2 to V3 is the same as defined in `Clang
Offload Bundler <https://clang.llvm.org/docs/ClangOffloadBundler.html>`_ except
when used in the :ref:`amdgpu-assembler-directive-amdgcn-target` assembler
directive and the bundle entry ID. In those cases it has the following BNF
syntax:

.. code::

  <target-id> ::== <processor> ( "+" <target-feature> )*

Where a target feature is omitted if *Off* and present if *On* or *Any*.

.. note::

  The code object V2 to V3 cannot represent *Any* and treats it the same as
  *On*.

.. _amdgpu-embedding-bundled-objects:

Embedding Bundled Code Objects
------------------------------

AMDGPU supports the HIP and OpenMP languages that perform code object embedding
as described in `Clang Offload Bundler
<https://clang.llvm.org/docs/ClangOffloadBundler.html>`_.

.. note::

  The target ID syntax used for code object V2 to V3 for a bundle entry ID
  differs from that used elsewhere. See :ref:`amdgpu-target-id-v2-v3`.

.. _amdgpu-address-spaces:

Address Spaces
--------------

The AMDGPU architecture supports a number of memory address spaces. The address
space names use the OpenCL standard names, with some additions.

The AMDGPU address spaces correspond to target architecture specific LLVM
address space numbers used in LLVM IR.

The AMDGPU address spaces are described in
:ref:`amdgpu-address-spaces-table`. Only 64-bit process address spaces are
supported for the ``amdgcn`` target.

  .. table:: AMDGPU Address Spaces
     :name: amdgpu-address-spaces-table

     ================================= =============== =========== ================ ======= ============================
     ..                                                                                     64-Bit Process Address Space
     --------------------------------- --------------- ----------- ---------------- ------------------------------------
     Address Space Name                LLVM IR Address HSA Segment Hardware         Address NULL Value
                                       Space Number    Name        Name             Size
     ================================= =============== =========== ================ ======= ============================
     Generic                           0               flat        flat             64      0x0000000000000000
     Global                            1               global      global           64      0x0000000000000000
     Region                            2               N/A         GDS              32      *not implemented for AMDHSA*
     Local                             3               group       LDS              32      0xFFFFFFFF
     Constant                          4               constant    *same as global* 64      0x0000000000000000
     Private                           5               private     scratch          32      0xFFFFFFFF
     Constant 32-bit                   6               *TODO*                               0x00000000
     Buffer Fat Pointer (experimental) 7               *TODO*
     Buffer Resource (experimental)    8               *TODO*
     Streamout Registers               128             N/A         GS_REGS
     ================================= =============== =========== ================ ======= ============================

**Generic**
  The generic address space is supported unless the *Target Properties* column
  of :ref:`amdgpu-processor-table` specifies *Does not support generic address
  space*.

  The generic address space uses the hardware flat address support for two fixed
  ranges of virtual addresses (the private and local apertures), that are
  outside the range of addressable global memory, to map from a flat address to
  a private or local address. This uses FLAT instructions that can take a flat
  address and access global, private (scratch), and group (LDS) memory depending
  on if the address is within one of the aperture ranges.

  Flat access to scratch requires hardware aperture setup and setup in the
  kernel prologue (see :ref:`amdgpu-amdhsa-kernel-prolog-flat-scratch`). Flat
  access to LDS requires hardware aperture setup and M0 (GFX7-GFX8) register
  setup (see :ref:`amdgpu-amdhsa-kernel-prolog-m0`).

  To convert between a private or group address space address (termed a segment
  address) and a flat address the base address of the corresponding aperture
  can be used. For GFX7-GFX8 these are available in the
  :ref:`amdgpu-amdhsa-hsa-aql-queue` the address of which can be obtained with
  Queue Ptr SGPR (see :ref:`amdgpu-amdhsa-initial-kernel-execution-state`). For
  GFX9-GFX11 the aperture base addresses are directly available as inline
  constant registers ``SRC_SHARED_BASE/LIMIT`` and ``SRC_PRIVATE_BASE/LIMIT``.
  In 64-bit address mode the aperture sizes are 2^32 bytes and the base is
  aligned to 2^32 which makes it easier to convert from flat to segment or
  segment to flat.

  A global address space address has the same value when used as a flat address
  so no conversion is needed.

**Global and Constant**
  The global and constant address spaces both use global virtual addresses,
  which are the same virtual address space used by the CPU. However, some
  virtual addresses may only be accessible to the CPU, some only accessible
  by the GPU, and some by both.

  Using the constant address space indicates that the data will not change
  during the execution of the kernel. This allows scalar read instructions to
  be used. As the constant address space could only be modified on the host
  side, a generic pointer loaded from the constant address space is safe to be
  assumed as a global pointer since only the device global memory is visible
  and managed on the host side. The vector and scalar L1 caches are invalidated
  of volatile data before each kernel dispatch execution to allow constant
  memory to change values between kernel dispatches.

**Region**
  The region address space uses the hardware Global Data Store (GDS). All
  wavefronts executing on the same device will access the same memory for any
  given region address. However, the same region address accessed by wavefronts
  executing on different devices will access different memory. It is higher
  performance than global memory. It is allocated by the runtime. The data
  store (DS) instructions can be used to access it.

**Local**
  The local address space uses the hardware Local Data Store (LDS) which is
  automatically allocated when the hardware creates the wavefronts of a
  work-group, and freed when all the wavefronts of a work-group have
  terminated. All wavefronts belonging to the same work-group will access the
  same memory for any given local address. However, the same local address
  accessed by wavefronts belonging to different work-groups will access
  different memory. It is higher performance than global memory. The data store
  (DS) instructions can be used to access it.

**Private**
  The private address space uses the hardware scratch memory support which
  automatically allocates memory when it creates a wavefront and frees it when
  a wavefronts terminates. The memory accessed by a lane of a wavefront for any
  given private address will be different to the memory accessed by another lane
  of the same or different wavefront for the same private address.

  If a kernel dispatch uses scratch, then the hardware allocates memory from a
  pool of backing memory allocated by the runtime for each wavefront. The lanes
  of the wavefront access this using dword (4 byte) interleaving. The mapping
  used from private address to backing memory address is:

    ``wavefront-scratch-base +
    ((private-address / 4) * wavefront-size * 4) +
    (wavefront-lane-id * 4) + (private-address % 4)``

  If each lane of a wavefront accesses the same private address, the
  interleaving results in adjacent dwords being accessed and hence requires
  fewer cache lines to be fetched.

  There are different ways that the wavefront scratch base address is
  determined by a wavefront (see
  :ref:`amdgpu-amdhsa-initial-kernel-execution-state`).

  Scratch memory can be accessed in an interleaved manner using buffer
  instructions with the scratch buffer descriptor and per wavefront scratch
  offset, by the scratch instructions, or by flat instructions. Multi-dword
  access is not supported except by flat and scratch instructions in
  GFX9-GFX11.

  Code that manipulates the stack values in other lanes of a wavefront,
  such as by ``addrspacecast``-ing stack pointers to generic ones and taking offsets
  that reach other lanes or by explicitly constructing the scratch buffer descriptor,
  triggers undefined behavior when it modifies the scratch values of other lanes.
  The compiler may assume that such modifications do not occur.

**Constant 32-bit**
  *TODO*

**Buffer Fat Pointer**
  The buffer fat pointer is an experimental address space that is currently
  unsupported in the backend. It exposes a non-integral pointer that is in
  the future intended to support the modelling of 128-bit buffer descriptors
  plus a 32-bit offset into the buffer (in total encapsulating a 160-bit
  *pointer*), allowing normal LLVM load/store/atomic operations to be used to
  model the buffer descriptors used heavily in graphics workloads targeting
  the backend.

  The buffer descriptor used to construct a buffer fat pointer must be *raw*:
  the stride must be 0, the "add tid" flag bust be 0, the swizzle enable bits
  must be off, and the extent must be measured in bytes. (On subtargets where
  bounds checking may be disabled, buffer fat pointers may choose to enable
  it or not).

**Buffer Resource**
  The buffer resource pointer, in address space 8, is the newer form
  for representing buffer descriptors in AMDGPU IR, replacing their
  previous representation as `<4 x i32>`. It is a non-integral pointer
  that represents a 128-bit buffer descriptor resource (`V#`).

  Since, in general, a buffer resource supports complex addressing modes that cannot
  be easily represented in LLVM (such as implicit swizzled access to structured
  buffers), it is **illegal** to perform non-trivial address computations, such as
  ``getelementptr`` operations, on buffer resources. They may be passed to
  AMDGPU buffer intrinsics, and they may be converted to and from ``i128``.

  Casting a buffer resource to a buffer fat pointer is permitted and adds an offset
  of 0.

  Buffer resources can be created from 64-bit pointers (which should be either
  generic or global) using the `llvm.amdgcn.make.buffer.rsrc` intrinsic, which
  takes the pointer, which becomes the base of the resource,
  the 16-bit stride (and swzizzle control) field stored in bits `63:48` of a `V#`,
  the 32-bit NumRecords/extent field (bits `95:64`), and the 32-bit flags field
  (bits `127:96`). The specific interpretation of these fields varies by the
  target architecture and is detailed in the ISA descriptions.

**Streamout Registers**
  Dedicated registers used by the GS NGG Streamout Instructions. The register
  file is modelled as a memory in a distinct address space because it is indexed
  by an address-like offset in place of named registers, and because register
  accesses affect LGKMcnt. This is an internal address space used only by the
  compiler. Do not use this address space for IR pointers.

.. _amdgpu-memory-scopes:

Memory Scopes
-------------

This section provides LLVM memory synchronization scopes supported by the AMDGPU
backend memory model when the target triple OS is ``amdhsa`` (see
:ref:`amdgpu-amdhsa-memory-model` and :ref:`amdgpu-target-triples`).

The memory model supported is based on the HSA memory model [HSA]_ which is
based in turn on HRF-indirect with scope inclusion [HRF]_. The happens-before
relation is transitive over the synchronizes-with relation independent of scope
and synchronizes-with allows the memory scope instances to be inclusive (see
table :ref:`amdgpu-amdhsa-llvm-sync-scopes-table`).

This is different to the OpenCL [OpenCL]_ memory model which does not have scope
inclusion and requires the memory scopes to exactly match. However, this
is conservatively correct for OpenCL.

  .. table:: AMDHSA LLVM Sync Scopes
     :name: amdgpu-amdhsa-llvm-sync-scopes-table

     ======================= ===================================================
     LLVM Sync Scope         Description
     ======================= ===================================================
     *none*                  The default: ``system``.

                             Synchronizes with, and participates in modification
                             and seq_cst total orderings with, other operations
                             (except image operations) for all address spaces
                             (except private, or generic that accesses private)
                             provided the other operation's sync scope is:

                             - ``system``.
                             - ``agent`` and executed by a thread on the same
                               agent.
                             - ``workgroup`` and executed by a thread in the
                               same work-group.
                             - ``wavefront`` and executed by a thread in the
                               same wavefront.

     ``agent``               Synchronizes with, and participates in modification
                             and seq_cst total orderings with, other operations
                             (except image operations) for all address spaces
                             (except private, or generic that accesses private)
                             provided the other operation's sync scope is:

                             - ``system`` or ``agent`` and executed by a thread
                               on the same agent.
                             - ``workgroup`` and executed by a thread in the
                               same work-group.
                             - ``wavefront`` and executed by a thread in the
                               same wavefront.

     ``workgroup``           Synchronizes with, and participates in modification
                             and seq_cst total orderings with, other operations
                             (except image operations) for all address spaces
                             (except private, or generic that accesses private)
                             provided the other operation's sync scope is:

                             - ``system``, ``agent`` or ``workgroup`` and
                               executed by a thread in the same work-group.
                             - ``wavefront`` and executed by a thread in the
                               same wavefront.

     ``wavefront``           Synchronizes with, and participates in modification
                             and seq_cst total orderings with, other operations
                             (except image operations) for all address spaces
                             (except private, or generic that accesses private)
                             provided the other operation's sync scope is:

                             - ``system``, ``agent``, ``workgroup`` or
                               ``wavefront`` and executed by a thread in the
                               same wavefront.

     ``singlethread``        Only synchronizes with and participates in
                             modification and seq_cst total orderings with,
                             other operations (except image operations) running
                             in the same thread for all address spaces (for
                             example, in signal handlers).

     ``one-as``              Same as ``system`` but only synchronizes with other
                             operations within the same address space.

     ``agent-one-as``        Same as ``agent`` but only synchronizes with other
                             operations within the same address space.

     ``workgroup-one-as``    Same as ``workgroup`` but only synchronizes with
                             other operations within the same address space.

     ``wavefront-one-as``    Same as ``wavefront`` but only synchronizes with
                             other operations within the same address space.

     ``singlethread-one-as`` Same as ``singlethread`` but only synchronizes with
                             other operations within the same address space.
     ======================= ===================================================

LLVM IR Intrinsics
------------------

The AMDGPU backend implements the following LLVM IR intrinsics.

*This section is WIP.*

.. table:: AMDGPU LLVM IR Intrinsics
  :name: amdgpu-llvm-ir-intrinsics-table

  ==============================================   ==========================================================
  LLVM Intrinsic                                   Description
  ==============================================   ==========================================================
  llvm.amdgcn.sqrt                                 Provides direct access to v_sqrt_f64, v_sqrt_f32 and v_sqrt_f16
                                                   (on targets with half support). Peforms sqrt function.

  llvm.amdgcn.log                                  Provides direct access to v_log_f32 and v_log_f16
                                                   (on targets with half support). Peforms log2 function.

  llvm.amdgcn.exp2                                 Provides direct access to v_exp_f32 and v_exp_f16
                                                   (on targets with half support). Performs exp2 function.

  :ref:`llvm.frexp <int_frexp>`                    Implemented for half, float and double.

  :ref:`llvm.log2 <int_log2>`                      Implemented for float and half (and vectors of float or
                                                   half). Not implemented for double. Hardware provides
                                                   1ULP accuracy for float, and 0.51ULP for half. Float
                                                   instruction does not natively support denormal
                                                   inputs.

  :ref:`llvm.sqrt <int_sqrt>`                      Implemented for double, float and half (and vectors).

  :ref:`llvm.log <int_log>`                        Implemented for float and half (and vectors).

  :ref:`llvm.exp <int_exp>`                        Implemented for float and half (and vectors).

  :ref:`llvm.log10 <int_log10>`                    Implemented for float and half (and vectors).

  :ref:`llvm.exp2 <int_exp2>`                      Implemented for float and half (and vectors of float or
                                                   half). Not implemented for double. Hardware provides
                                                   1ULP accuracy for float, and 0.51ULP for half. Float
                                                   instruction does not natively support denormal
                                                   inputs.

  :ref:`llvm.stacksave.p5 <int_stacksave>`         Implemented, must use the alloca address space.
  :ref:`llvm.stackrestore.p5 <int_stackrestore>`   Implemented, must use the alloca address space.

  llvm.amdgcn.wave.reduce.umin                     Performs an arithmetic unsigned min reduction on the unsigned values
                                                   provided by each lane in the wavefront.
                                                   Intrinsic takes a hint for reduction strategy using second operand
                                                   0: Target default preference,
                                                   1: `Iterative strategy`, and
                                                   2: `DPP`.
                                                   If target does not support the DPP operations (e.g. gfx6/7),
                                                   reduction will be performed using default iterative strategy.
                                                   Intrinsic is currently only implemented for i32.

  llvm.amdgcn.wave.reduce.umax                     Performs an arithmetic unsigned max reduction on the unsigned values
                                                   provided by each lane in the wavefront.
                                                   Intrinsic takes a hint for reduction strategy using second operand
                                                   0: Target default preference,
                                                   1: `Iterative strategy`, and
                                                   2: `DPP`.
                                                   If target does not support the DPP operations (e.g. gfx6/7),
                                                   reduction will be performed using default iterative strategy.
                                                   Intrinsic is currently only implemented for i32.

  ==============================================   ==========================================================

.. TODO::

   List AMDGPU intrinsics.

LLVM IR Attributes
------------------

The AMDGPU backend supports the following LLVM IR attributes.

  .. table:: AMDGPU LLVM IR Attributes
     :name: amdgpu-llvm-ir-attributes-table

     ======================================= ==========================================================
     LLVM Attribute                          Description
     ======================================= ==========================================================
     "amdgpu-flat-work-group-size"="min,max" Specify the minimum and maximum flat work group sizes that
                                             will be specified when the kernel is dispatched. Generated
                                             by the ``amdgpu_flat_work_group_size`` CLANG attribute [CLANG-ATTR]_.
                                             The IR implied default value is 1,1024. Clang may emit this attribute
                                             with more restrictive bounds depending on language defaults.
                                             If the actual block or workgroup size exceeds the limit at any point during
                                             the execution, the behavior is undefined. For example, even if there is
                                             only one active thread but the thread local id exceeds the limit, the
                                             behavior is undefined.

     "amdgpu-implicitarg-num-bytes"="n"      Number of kernel argument bytes to add to the kernel
                                             argument block size for the implicit arguments. This
                                             varies by OS and language (for OpenCL see
                                             :ref:`opencl-kernel-implicit-arguments-appended-for-amdhsa-os-table`).
     "amdgpu-num-sgpr"="n"                   Specifies the number of SGPRs to use. Generated by
                                             the ``amdgpu_num_sgpr`` CLANG attribute [CLANG-ATTR]_.
     "amdgpu-num-vgpr"="n"                   Specifies the number of VGPRs to use. Generated by the
                                             ``amdgpu_num_vgpr`` CLANG attribute [CLANG-ATTR]_.
     "amdgpu-waves-per-eu"="m,n"             Specify the minimum and maximum number of waves per
                                             execution unit. Generated by the ``amdgpu_waves_per_eu``
                                             CLANG attribute [CLANG-ATTR]_. This is an optimization hint,
                                             and the backend may not be able to satisfy the request. If
                                             the specified range is incompatible with the function's
                                             "amdgpu-flat-work-group-size" value, the implied occupancy
                                             bounds by the workgroup size takes precedence.

     "amdgpu-ieee" true/false.               Specify whether the function expects the IEEE field of the
                                             mode register to be set on entry. Overrides the default for
                                             the calling convention.
     "amdgpu-dx10-clamp" true/false.         Specify whether the function expects the DX10_CLAMP field of
                                             the mode register to be set on entry. Overrides the default
                                             for the calling convention.

     "amdgpu-no-workitem-id-x"               Indicates the function does not depend on the value of the
                                             llvm.amdgcn.workitem.id.x intrinsic. If a function is marked with this
                                             attribute, or reached through a call site marked with this attribute,
                                             the value returned by the intrinsic is undefined. The backend can
                                             generally infer this during code generation, so typically there is no
                                             benefit to frontends marking functions with this.

     "amdgpu-no-workitem-id-y"               The same as amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.workitem.id.y intrinsic.

     "amdgpu-no-workitem-id-z"               The same as amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.workitem.id.z intrinsic.

     "amdgpu-no-workgroup-id-x"              The same as amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.workgroup.id.x intrinsic.

     "amdgpu-no-workgroup-id-y"              The same as amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.workgroup.id.y intrinsic.

     "amdgpu-no-workgroup-id-z"              The same as amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.workgroup.id.z intrinsic.

     "amdgpu-no-dispatch-ptr"                The same as amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.dispatch.ptr intrinsic.

     "amdgpu-no-implicitarg-ptr"             The same as amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.implicitarg.ptr intrinsic.

     "amdgpu-no-dispatch-id"                 The same as amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.dispatch.id intrinsic.

     "amdgpu-no-queue-ptr"                   Similar to amdgpu-no-workitem-id-x, except for the
                                             llvm.amdgcn.queue.ptr intrinsic. Note that unlike the other ABI hint
                                             attributes, the queue pointer may be required in situations where the
                                             intrinsic call does not directly appear in the program. Some subtargets
                                             require the queue pointer for to handle some addrspacecasts, as well
                                             as the llvm.amdgcn.is.shared, llvm.amdgcn.is.private, llvm.trap, and
                                             llvm.debug intrinsics.

     "amdgpu-no-hostcall-ptr"                Similar to amdgpu-no-implicitarg-ptr, except specific to the implicit
                                             kernel argument that holds the pointer to the hostcall buffer. If this
                                             attribute is absent, then the amdgpu-no-implicitarg-ptr is also removed.

     "amdgpu-no-heap-ptr"                    Similar to amdgpu-no-implicitarg-ptr, except specific to the implicit
                                             kernel argument that holds the pointer to an initialized memory buffer
                                             that conforms to the requirements of the malloc/free device library V1
                                             version implementation. If this attribute is absent, then the
                                             amdgpu-no-implicitarg-ptr is also removed.

     "amdgpu-no-multigrid-sync-arg"          Similar to amdgpu-no-implicitarg-ptr, except specific to the implicit
                                             kernel argument that holds the multigrid synchronization pointer. If this
                                             attribute is absent, then the amdgpu-no-implicitarg-ptr is also removed.

     "amdgpu-no-default-queue"               Similar to amdgpu-no-implicitarg-ptr, except specific to the implicit
                                             kernel argument that holds the default queue pointer. If this
                                             attribute is absent, then the amdgpu-no-implicitarg-ptr is also removed.

     "amdgpu-no-completion-action"           Similar to amdgpu-no-implicitarg-ptr, except specific to the implicit
                                             kernel argument that holds the completion action pointer. If this
                                             attribute is absent, then the amdgpu-no-implicitarg-ptr is also removed.

     "amdgpu-lds-size"="min[,max]"           Min is the minimum number of bytes that will be allocated in the Local
                                             Data Store at address zero. Variables are allocated within this frame
                                             using absolute symbol metadata, primarily by the AMDGPULowerModuleLDS
                                             pass. Optional max is the maximum number of bytes that will be allocated.
                                             Note that min==max indicates that no further variables can be added to
                                             the frame. This is an internal detail of how LDS variables are lowered,
                                             language front ends should not set this attribute.

     ======================================= ==========================================================

Calling Conventions
-------------------

The AMDGPU backend supports the following calling conventions:

  .. table:: AMDGPU Calling Conventions
     :name: amdgpu-cc

     =============================== ==========================================================
     Calling Convention              Description
     =============================== ==========================================================
     ``ccc``                         The C calling convention. Used by default.
                                     See :ref:`amdgpu-amdhsa-function-call-convention-non-kernel-functions`
                                     for more details.

     ``fastcc``                      The fast calling convention. Mostly the same as the ``ccc``.

     ``coldcc``                      The cold calling convention. Mostly the same as the ``ccc``.

     ``amdgpu_cs``                   Used for Mesa/AMDPAL compute shaders.
                                     ..TODO::
                                     Describe.

     ``amdgpu_cs_chain``             Similar to ``amdgpu_cs``, with differences described below.

                                     Functions with this calling convention cannot be called directly. They must
                                     instead be launched via the ``llvm.amdgcn.cs.chain`` intrinsic.

                                     Arguments are passed in SGPRs, starting at s0, if they have the ``inreg``
                                     attribute, and in VGPRs otherwise, starting at v8. Using more SGPRs or VGPRs
                                     than available in the subtarget is not allowed.  On subtargets that use
                                     a scratch buffer descriptor (as opposed to ``scratch_{load,store}_*`` instructions),
                                     the scratch buffer descriptor is passed in s[48:51]. This limits the
                                     SGPR / ``inreg`` arguments to the equivalent of 48 dwords; using more
                                     than that is not allowed.

                                     The return type must be void.
                                     Varargs, sret, byval, byref, inalloca, preallocated are not supported.

                                     Values in scalar registers as well as v0-v7 are not preserved. Values in
                                     VGPRs starting at v8 are not preserved for the active lanes, but must be
                                     saved by the callee for inactive lanes when using WWM.

                                     Wave scratch is "empty" at function boundaries. There is no stack pointer input
                                     or output value, but functions are free to use scratch starting from an initial
                                     stack pointer. Calls to ``amdgpu_gfx`` functions are allowed and behave like they
                                     do in ``amdgpu_cs`` functions.

                                     All counters (``lgkmcnt``, ``vmcnt``, ``storecnt``, etc.) are presumed in an
                                     unknown state at function entry.

                                     A function may have multiple exits (e.g. one chain exit and one plain ``ret void``
                                     for when the wave ends), but all ``llvm.amdgcn.cs.chain`` exits must be in
                                     uniform control flow.

     ``amdgpu_cs_chain_preserve``    Same as ``amdgpu_cs_chain``, but active lanes for VGPRs starting at v8 are preserved.

     ``amdgpu_es``                   Used for AMDPAL shader stage before geometry shader if geometry is in
                                     use. So either the domain (= tessellation evaluation) shader if
                                     tessellation is in use, or otherwise the vertex shader.
                                     ..TODO::
                                     Describe.

     ``amdgpu_gfx``                  Used for AMD graphics targets. Functions with this calling convention
                                     cannot be used as entry points.
                                     ..TODO::
                                     Describe.

     ``amdgpu_gs``                   Used for Mesa/AMDPAL geometry shaders.
                                     ..TODO::
                                     Describe.

     ``amdgpu_hs``                   Used for Mesa/AMDPAL hull shaders (= tessellation control shaders).
                                     ..TODO::
                                     Describe.

     ``amdgpu_kernel``               See :ref:`amdgpu-amdhsa-function-call-convention-kernel-functions`

     ``amdgpu_ls``                   Used for AMDPAL vertex shader if tessellation is in use.
                                     ..TODO::
                                     Describe.

     ``amdgpu_ps``                   Used for Mesa/AMDPAL pixel shaders.
                                     ..TODO::
                                     Describe.

     ``amdgpu_vs``                   Used for Mesa/AMDPAL last shader stage before rasterization (vertex
                                     shader if tessellation and geometry are not in use, or otherwise
                                     copy shader if one is needed).
                                     ..TODO::
                                     Describe.

     =============================== ==========================================================


.. _amdgpu-elf-code-object:

ELF Code Object
===============

The AMDGPU backend generates a standard ELF [ELF]_ relocatable code object that
can be linked by ``lld`` to produce a standard ELF shared code object which can
be loaded and executed on an AMDGPU target.

.. _amdgpu-elf-header:

Header
------

The AMDGPU backend uses the following ELF header:

  .. table:: AMDGPU ELF Header
     :name: amdgpu-elf-header-table

     ========================== ===============================
     Field                      Value
     ========================== ===============================
     ``e_ident[EI_CLASS]``      ``ELFCLASS64``
     ``e_ident[EI_DATA]``       ``ELFDATA2LSB``
     ``e_ident[EI_OSABI]``      - ``ELFOSABI_NONE``
                                - ``ELFOSABI_AMDGPU_HSA``
                                - ``ELFOSABI_AMDGPU_PAL``
                                - ``ELFOSABI_AMDGPU_MESA3D``
     ``e_ident[EI_ABIVERSION]`` - ``ELFABIVERSION_AMDGPU_HSA_V2``
                                - ``ELFABIVERSION_AMDGPU_HSA_V3``
                                - ``ELFABIVERSION_AMDGPU_HSA_V4``
                                - ``ELFABIVERSION_AMDGPU_HSA_V5``
                                - ``ELFABIVERSION_AMDGPU_PAL``
                                - ``ELFABIVERSION_AMDGPU_MESA3D``
     ``e_type``                 - ``ET_REL``
                                - ``ET_DYN``
     ``e_machine``              ``EM_AMDGPU``
     ``e_entry``                0
     ``e_flags``                See :ref:`amdgpu-elf-header-e_flags-v2-table`,
                                :ref:`amdgpu-elf-header-e_flags-table-v3`,
                                and :ref:`amdgpu-elf-header-e_flags-table-v4-onwards`
     ========================== ===============================

..

  .. table:: AMDGPU ELF Header Enumeration Values
     :name: amdgpu-elf-header-enumeration-values-table

     =============================== =====
     Name                            Value
     =============================== =====
     ``EM_AMDGPU``                   224
     ``ELFOSABI_NONE``               0
     ``ELFOSABI_AMDGPU_HSA``         64
     ``ELFOSABI_AMDGPU_PAL``         65
     ``ELFOSABI_AMDGPU_MESA3D``      66
     ``ELFABIVERSION_AMDGPU_HSA_V2`` 0
     ``ELFABIVERSION_AMDGPU_HSA_V3`` 1
     ``ELFABIVERSION_AMDGPU_HSA_V4`` 2
     ``ELFABIVERSION_AMDGPU_HSA_V5`` 3
     ``ELFABIVERSION_AMDGPU_PAL``    0
     ``ELFABIVERSION_AMDGPU_MESA3D`` 0
     =============================== =====

``e_ident[EI_CLASS]``
  The ELF class is:

  * ``ELFCLASS32`` for ``r600`` architecture.

  * ``ELFCLASS64`` for ``amdgcn`` architecture which only supports 64-bit
    process address space applications.

``e_ident[EI_DATA]``
  All AMDGPU targets use ``ELFDATA2LSB`` for little-endian byte ordering.

``e_ident[EI_OSABI]``
  One of the following AMDGPU target architecture specific OS ABIs
  (see :ref:`amdgpu-os`):

  * ``ELFOSABI_NONE`` for *unknown* OS.

  * ``ELFOSABI_AMDGPU_HSA`` for ``amdhsa`` OS.

  * ``ELFOSABI_AMDGPU_PAL`` for ``amdpal`` OS.

  * ``ELFOSABI_AMDGPU_MESA3D`` for ``mesa3D`` OS.

``e_ident[EI_ABIVERSION]``
  The ABI version of the AMDGPU target architecture specific OS ABI to which the code
  object conforms:

  * ``ELFABIVERSION_AMDGPU_HSA_V2`` is used to specify the version of AMD HSA
    runtime ABI for code object V2. Specify using the Clang option
    ``-mcode-object-version=2``.

  * ``ELFABIVERSION_AMDGPU_HSA_V3`` is used to specify the version of AMD HSA
    runtime ABI for code object V3. Specify using the Clang option
    ``-mcode-object-version=3``.

  * ``ELFABIVERSION_AMDGPU_HSA_V4`` is used to specify the version of AMD HSA
    runtime ABI for code object V4. Specify using the Clang option
    ``-mcode-object-version=4``. This is the default code object
    version if not specified.

  * ``ELFABIVERSION_AMDGPU_HSA_V5`` is used to specify the version of AMD HSA
    runtime ABI for code object V5. Specify using the Clang option
    ``-mcode-object-version=5``.

  * ``ELFABIVERSION_AMDGPU_PAL`` is used to specify the version of AMD PAL
    runtime ABI.

  * ``ELFABIVERSION_AMDGPU_MESA3D`` is used to specify the version of AMD MESA
    3D runtime ABI.

``e_type``
  Can be one of the following values:


  ``ET_REL``
    The type produced by the AMDGPU backend compiler as it is relocatable code
    object.

  ``ET_DYN``
    The type produced by the linker as it is a shared code object.

  The AMD HSA runtime loader requires a ``ET_DYN`` code object.

``e_machine``
  The value ``EM_AMDGPU`` is used for the machine for all processors supported
  by the ``r600`` and ``amdgcn`` architectures (see
  :ref:`amdgpu-processor-table`). The specific processor is specified in the
  ``NT_AMD_HSA_ISA_VERSION`` note record for code object V2 (see
  :ref:`amdgpu-note-records-v2`) and in the ``EF_AMDGPU_MACH`` bit field of the
  ``e_flags`` for code object V3 and above (see
  :ref:`amdgpu-elf-header-e_flags-table-v3` and
  :ref:`amdgpu-elf-header-e_flags-table-v4-onwards`).

``e_entry``
  The entry point is 0 as the entry points for individual kernels must be
  selected in order to invoke them through AQL packets.

``e_flags``
  The AMDGPU backend uses the following ELF header flags:

  .. table:: AMDGPU ELF Header ``e_flags`` for Code Object V2
     :name: amdgpu-elf-header-e_flags-v2-table

     ===================================== ===== =============================
     Name                                  Value Description
     ===================================== ===== =============================
     ``EF_AMDGPU_FEATURE_XNACK_V2``        0x01  Indicates if the ``xnack``
                                                 target feature is
                                                 enabled for all code
                                                 contained in the code object.
                                                 If the processor
                                                 does not support the
                                                 ``xnack`` target
                                                 feature then must
                                                 be 0.
                                                 See
                                                 :ref:`amdgpu-target-features`.
     ``EF_AMDGPU_FEATURE_TRAP_HANDLER_V2`` 0x02  Indicates if the trap
                                                 handler is enabled for all
                                                 code contained in the code
                                                 object. If the processor
                                                 does not support a trap
                                                 handler then must be 0.
                                                 See
                                                 :ref:`amdgpu-target-features`.
     ===================================== ===== =============================

  .. table:: AMDGPU ELF Header ``e_flags`` for Code Object V3
     :name: amdgpu-elf-header-e_flags-table-v3

     ================================= ===== =============================
     Name                              Value Description
     ================================= ===== =============================
     ``EF_AMDGPU_MACH``                0x0ff AMDGPU processor selection
                                             mask for
                                             ``EF_AMDGPU_MACH_xxx`` values
                                             defined in
                                             :ref:`amdgpu-ef-amdgpu-mach-table`.
     ``EF_AMDGPU_FEATURE_XNACK_V3``    0x100 Indicates if the ``xnack``
                                             target feature is
                                             enabled for all code
                                             contained in the code object.
                                             If the processor
                                             does not support the
                                             ``xnack`` target
                                             feature then must
                                             be 0.
                                             See
                                             :ref:`amdgpu-target-features`.
     ``EF_AMDGPU_FEATURE_SRAMECC_V3``  0x200 Indicates if the ``sramecc``
                                             target feature is
                                             enabled for all code
                                             contained in the code object.
                                             If the processor
                                             does not support the
                                             ``sramecc`` target
                                             feature then must
                                             be 0.
                                             See
                                             :ref:`amdgpu-target-features`.
     ================================= ===== =============================

  .. table:: AMDGPU ELF Header ``e_flags`` for Code Object V4 and After
     :name: amdgpu-elf-header-e_flags-table-v4-onwards

     ============================================ ===== ===================================
     Name                                         Value      Description
     ============================================ ===== ===================================
     ``EF_AMDGPU_MACH``                           0x0ff AMDGPU processor selection
                                                        mask for
                                                        ``EF_AMDGPU_MACH_xxx`` values
                                                        defined in
                                                        :ref:`amdgpu-ef-amdgpu-mach-table`.
     ``EF_AMDGPU_FEATURE_XNACK_V4``               0x300 XNACK selection mask for
                                                        ``EF_AMDGPU_FEATURE_XNACK_*_V4``
                                                        values.
     ``EF_AMDGPU_FEATURE_XNACK_UNSUPPORTED_V4``   0x000 XNACK unsupported.
     ``EF_AMDGPU_FEATURE_XNACK_ANY_V4``           0x100 XNACK can have any value.
     ``EF_AMDGPU_FEATURE_XNACK_OFF_V4``           0x200 XNACK disabled.
     ``EF_AMDGPU_FEATURE_XNACK_ON_V4``            0x300 XNACK enabled.
     ``EF_AMDGPU_FEATURE_SRAMECC_V4``             0xc00 SRAMECC selection mask for
                                                        ``EF_AMDGPU_FEATURE_SRAMECC_*_V4``
                                                        values.
     ``EF_AMDGPU_FEATURE_SRAMECC_UNSUPPORTED_V4`` 0x000 SRAMECC unsupported.
     ``EF_AMDGPU_FEATURE_SRAMECC_ANY_V4``         0x400 SRAMECC can have any value.
     ``EF_AMDGPU_FEATURE_SRAMECC_OFF_V4``         0x800 SRAMECC disabled,
     ``EF_AMDGPU_FEATURE_SRAMECC_ON_V4``          0xc00 SRAMECC enabled.
     ============================================ ===== ===================================

  .. table:: AMDGPU ``EF_AMDGPU_MACH`` Values
     :name: amdgpu-ef-amdgpu-mach-table

     ==================================== ========== =============================
     Name                                 Value      Description (see
                                                     :ref:`amdgpu-processor-table`)
     ==================================== ========== =============================
     ``EF_AMDGPU_MACH_NONE``              0x000      *not specified*
     ``EF_AMDGPU_MACH_R600_R600``         0x001      ``r600``
     ``EF_AMDGPU_MACH_R600_R630``         0x002      ``r630``
     ``EF_AMDGPU_MACH_R600_RS880``        0x003      ``rs880``
     ``EF_AMDGPU_MACH_R600_RV670``        0x004      ``rv670``
     ``EF_AMDGPU_MACH_R600_RV710``        0x005      ``rv710``
     ``EF_AMDGPU_MACH_R600_RV730``        0x006      ``rv730``
     ``EF_AMDGPU_MACH_R600_RV770``        0x007      ``rv770``
     ``EF_AMDGPU_MACH_R600_CEDAR``        0x008      ``cedar``
     ``EF_AMDGPU_MACH_R600_CYPRESS``      0x009      ``cypress``
     ``EF_AMDGPU_MACH_R600_JUNIPER``      0x00a      ``juniper``
     ``EF_AMDGPU_MACH_R600_REDWOOD``      0x00b      ``redwood``
     ``EF_AMDGPU_MACH_R600_SUMO``         0x00c      ``sumo``
     ``EF_AMDGPU_MACH_R600_BARTS``        0x00d      ``barts``
     ``EF_AMDGPU_MACH_R600_CAICOS``       0x00e      ``caicos``
     ``EF_AMDGPU_MACH_R600_CAYMAN``       0x00f      ``cayman``
     ``EF_AMDGPU_MACH_R600_TURKS``        0x010      ``turks``
     *reserved*                           0x011 -    Reserved for ``r600``
                                          0x01f      architecture processors.
     ``EF_AMDGPU_MACH_AMDGCN_GFX600``     0x020      ``gfx600``
     ``EF_AMDGPU_MACH_AMDGCN_GFX601``     0x021      ``gfx601``
     ``EF_AMDGPU_MACH_AMDGCN_GFX700``     0x022      ``gfx700``
     ``EF_AMDGPU_MACH_AMDGCN_GFX701``     0x023      ``gfx701``
     ``EF_AMDGPU_MACH_AMDGCN_GFX702``     0x024      ``gfx702``
     ``EF_AMDGPU_MACH_AMDGCN_GFX703``     0x025      ``gfx703``
     ``EF_AMDGPU_MACH_AMDGCN_GFX704``     0x026      ``gfx704``
     *reserved*                           0x027      Reserved.
     ``EF_AMDGPU_MACH_AMDGCN_GFX801``     0x028      ``gfx801``
     ``EF_AMDGPU_MACH_AMDGCN_GFX802``     0x029      ``gfx802``
     ``EF_AMDGPU_MACH_AMDGCN_GFX803``     0x02a      ``gfx803``
     ``EF_AMDGPU_MACH_AMDGCN_GFX810``     0x02b      ``gfx810``
     ``EF_AMDGPU_MACH_AMDGCN_GFX900``     0x02c      ``gfx900``
     ``EF_AMDGPU_MACH_AMDGCN_GFX902``     0x02d      ``gfx902``
     ``EF_AMDGPU_MACH_AMDGCN_GFX904``     0x02e      ``gfx904``
     ``EF_AMDGPU_MACH_AMDGCN_GFX906``     0x02f      ``gfx906``
     ``EF_AMDGPU_MACH_AMDGCN_GFX908``     0x030      ``gfx908``
     ``EF_AMDGPU_MACH_AMDGCN_GFX909``     0x031      ``gfx909``
     ``EF_AMDGPU_MACH_AMDGCN_GFX90C``     0x032      ``gfx90c``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1010``    0x033      ``gfx1010``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1011``    0x034      ``gfx1011``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1012``    0x035      ``gfx1012``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1030``    0x036      ``gfx1030``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1031``    0x037      ``gfx1031``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1032``    0x038      ``gfx1032``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1033``    0x039      ``gfx1033``
     ``EF_AMDGPU_MACH_AMDGCN_GFX602``     0x03a      ``gfx602``
     ``EF_AMDGPU_MACH_AMDGCN_GFX705``     0x03b      ``gfx705``
     ``EF_AMDGPU_MACH_AMDGCN_GFX805``     0x03c      ``gfx805``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1035``    0x03d      ``gfx1035``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1034``    0x03e      ``gfx1034``
     ``EF_AMDGPU_MACH_AMDGCN_GFX90A``     0x03f      ``gfx90a``
     ``EF_AMDGPU_MACH_AMDGCN_GFX940``     0x040      ``gfx940``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1100``    0x041      ``gfx1100``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1013``    0x042      ``gfx1013``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1150``    0x043      ``gfx1150``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1103``    0x044      ``gfx1103``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1036``    0x045      ``gfx1036``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1101``    0x046      ``gfx1101``
     ``EF_AMDGPU_MACH_AMDGCN_GFX1102``    0x047      ``gfx1102``
     *reserved*                           0x048      Reserved.
     *reserved*                           0x049      Reserved.
     ``EF_AMDGPU_MACH_AMDGCN_GFX1151``    0x04a      ``gfx1151``
     ``EF_AMDGPU_MACH_AMDGCN_GFX941``     0x04b      ``gfx941``
     ``EF_AMDGPU_MACH_AMDGCN_GFX942``     0x04c      ``gfx942``
     ==================================== ========== =============================

Sections
--------

An AMDGPU target ELF code object has the standard ELF sections which include:

  .. table:: AMDGPU ELF Sections
     :name: amdgpu-elf-sections-table

     ================== ================ =================================
     Name               Type             Attributes
     ================== ================ =================================
     ``.bss``           ``SHT_NOBITS``   ``SHF_ALLOC`` + ``SHF_WRITE``
     ``.data``          ``SHT_PROGBITS`` ``SHF_ALLOC`` + ``SHF_WRITE``
     ``.debug_``\ *\**  ``SHT_PROGBITS`` *none*
     ``.dynamic``       ``SHT_DYNAMIC``  ``SHF_ALLOC``
     ``.dynstr``        ``SHT_PROGBITS`` ``SHF_ALLOC``
     ``.dynsym``        ``SHT_PROGBITS`` ``SHF_ALLOC``
     ``.got``           ``SHT_PROGBITS`` ``SHF_ALLOC`` + ``SHF_WRITE``
     ``.hash``          ``SHT_HASH``     ``SHF_ALLOC``
     ``.note``          ``SHT_NOTE``     *none*
     ``.rela``\ *name*  ``SHT_RELA``     *none*
     ``.rela.dyn``      ``SHT_RELA``     *none*
     ``.rodata``        ``SHT_PROGBITS`` ``SHF_ALLOC``
     ``.shstrtab``      ``SHT_STRTAB``   *none*
     ``.strtab``        ``SHT_STRTAB``   *none*
     ``.symtab``        ``SHT_SYMTAB``   *none*
     ``.text``          ``SHT_PROGBITS`` ``SHF_ALLOC`` + ``SHF_EXECINSTR``
     ================== ================ =================================

These sections have their standard meanings (see [ELF]_) and are only generated
if needed.

``.debug``\ *\**
  The standard DWARF sections. See :ref:`amdgpu-dwarf-debug-information` for
  information on the DWARF produced by the AMDGPU backend.

``.dynamic``, ``.dynstr``, ``.dynsym``, ``.hash``
  The standard sections used by a dynamic loader.

``.note``
  See :ref:`amdgpu-note-records` for the note records supported by the AMDGPU
  backend.

``.rela``\ *name*, ``.rela.dyn``
  For relocatable code objects, *name* is the name of the section that the
  relocation records apply. For example, ``.rela.text`` is the section name for
  relocation records associated with the ``.text`` section.

  For linked shared code objects, ``.rela.dyn`` contains all the relocation
  records from each of the relocatable code object's ``.rela``\ *name* sections.

  See :ref:`amdgpu-relocation-records` for the relocation records supported by
  the AMDGPU backend.

``.text``
  The executable machine code for the kernels and functions they call. Generated
  as position independent code. See :ref:`amdgpu-code-conventions` for
  information on conventions used in the isa generation.

.. _amdgpu-note-records:

Note Records
------------

The AMDGPU backend code object contains ELF note records in the ``.note``
section. The set of generated notes and their semantics depend on the code
object version; see :ref:`amdgpu-note-records-v2` and
:ref:`amdgpu-note-records-v3-onwards`.

As required by ``ELFCLASS32`` and ``ELFCLASS64``, minimal zero-byte padding
must be generated after the ``name`` field to ensure the ``desc`` field is 4
byte aligned. In addition, minimal zero-byte padding must be generated to
ensure the ``desc`` field size is a multiple of 4 bytes. The ``sh_addralign``
field of the ``.note`` section must be at least 4 to indicate at least 8 byte
alignment.

.. _amdgpu-note-records-v2:

Code Object V2 Note Records
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. warning::
  Code object V2 is not the default code object version emitted by
  this version of LLVM.

The AMDGPU backend code object uses the following ELF note record in the
``.note`` section when compiling for code object V2.

The note record vendor field is "AMD".

Additional note records may be present, but any which are not documented here
are deprecated and should not be used.

  .. table:: AMDGPU Code Object V2 ELF Note Records
     :name: amdgpu-elf-note-records-v2-table

     ===== ===================================== ======================================
     Name  Type                                  Description
     ===== ===================================== ======================================
     "AMD" ``NT_AMD_HSA_CODE_OBJECT_VERSION``    Code object version.
     "AMD" ``NT_AMD_HSA_HSAIL``                  HSAIL properties generated by the HSAIL
                                                 Finalizer and not the LLVM compiler.
     "AMD" ``NT_AMD_HSA_ISA_VERSION``            Target ISA version.
     "AMD" ``NT_AMD_HSA_METADATA``               Metadata null terminated string in
                                                 YAML [YAML]_ textual format.
     "AMD" ``NT_AMD_HSA_ISA_NAME``               Target ISA name.
     ===== ===================================== ======================================

..

  .. table:: AMDGPU Code Object V2 ELF Note Record Enumeration Values
     :name: amdgpu-elf-note-record-enumeration-values-v2-table

     ===================================== =====
     Name                                  Value
     ===================================== =====
     ``NT_AMD_HSA_CODE_OBJECT_VERSION``    1
     ``NT_AMD_HSA_HSAIL``                  2
     ``NT_AMD_HSA_ISA_VERSION``            3
     *reserved*                            4-9
     ``NT_AMD_HSA_METADATA``               10
     ``NT_AMD_HSA_ISA_NAME``               11
     ===================================== =====

``NT_AMD_HSA_CODE_OBJECT_VERSION``
  Specifies the code object version number. The description field has the
  following layout:

  .. code:: c

    struct amdgpu_hsa_note_code_object_version_s {
      uint32_t major_version;
      uint32_t minor_version;
    };

  The ``major_version`` has a value less than or equal to 2.

``NT_AMD_HSA_HSAIL``
  Specifies the HSAIL properties used by the HSAIL Finalizer. The description
  field has the following layout:

  .. code:: c

    struct amdgpu_hsa_note_hsail_s {
      uint32_t hsail_major_version;
      uint32_t hsail_minor_version;
      uint8_t profile;
      uint8_t machine_model;
      uint8_t default_float_round;
    };

``NT_AMD_HSA_ISA_VERSION``
  Specifies the target ISA version. The description field has the following layout:

  .. code:: c

    struct amdgpu_hsa_note_isa_s {
      uint16_t vendor_name_size;
      uint16_t architecture_name_size;
      uint32_t major;
      uint32_t minor;
      uint32_t stepping;
      char vendor_and_architecture_name[1];
    };

  ``vendor_name_size`` and ``architecture_name_size`` are the length of the
  vendor and architecture names respectively, including the NUL character.

  ``vendor_and_architecture_name`` contains the NUL terminates string for the
  vendor, immediately followed by the NUL terminated string for the
  architecture.

  This note record is used by the HSA runtime loader.

  Code object V2 only supports a limited number of processors and has fixed
  settings for target features. See
  :ref:`amdgpu-elf-note-record-supported_processors-v2-table` for a list of
  processors and the corresponding target ID. In the table the note record ISA
  name is a concatenation of the vendor name, architecture name, major, minor,
  and stepping separated by a ":".

  The target ID column shows the processor name and fixed target features used
  by the LLVM compiler. The LLVM compiler does not generate a
  ``NT_AMD_HSA_HSAIL`` note record.

  A code object generated by the Finalizer also uses code object V2 and always
  generates a ``NT_AMD_HSA_HSAIL`` note record. The processor name and
  ``sramecc`` target feature is as shown in
  :ref:`amdgpu-elf-note-record-supported_processors-v2-table` but the ``xnack``
  target feature is specified by the ``EF_AMDGPU_FEATURE_XNACK_V2`` ``e_flags``
  bit.

``NT_AMD_HSA_ISA_NAME``
  Specifies the target ISA name as a non-NUL terminated string.

  This note record is not used by the HSA runtime loader.

  See the ``NT_AMD_HSA_ISA_VERSION`` note record description of the code object
  V2's limited support of processors and fixed settings for target features.

  See :ref:`amdgpu-elf-note-record-supported_processors-v2-table` for a mapping
  from the string to the corresponding target ID. If the ``xnack`` target
  feature is supported and enabled, the string produced by the LLVM compiler
  will may have a ``+xnack`` appended. The Finlizer did not do the appending and
  instead used the ``EF_AMDGPU_FEATURE_XNACK_V2`` ``e_flags`` bit.

``NT_AMD_HSA_METADATA``
  Specifies extensible metadata associated with the code objects executed on HSA
  [HSA]_ compatible runtimes (see :ref:`amdgpu-os`). It is required when the
  target triple OS is ``amdhsa`` (see :ref:`amdgpu-target-triples`). See
  :ref:`amdgpu-amdhsa-code-object-metadata-v2` for the syntax of the code object
  metadata string.

  .. table:: AMDGPU Code Object V2 Supported Processors and Fixed Target Feature Settings
     :name: amdgpu-elf-note-record-supported_processors-v2-table

     ===================== ==========================
     Note Record ISA Name  Target ID
     ===================== ==========================
     ``AMD:AMDGPU:6:0:0``  ``gfx600``
     ``AMD:AMDGPU:6:0:1``  ``gfx601``
     ``AMD:AMDGPU:6:0:2``  ``gfx602``
     ``AMD:AMDGPU:7:0:0``  ``gfx700``
     ``AMD:AMDGPU:7:0:1``  ``gfx701``
     ``AMD:AMDGPU:7:0:2``  ``gfx702``
     ``AMD:AMDGPU:7:0:3``  ``gfx703``
     ``AMD:AMDGPU:7:0:4``  ``gfx704``
     ``AMD:AMDGPU:7:0:5``  ``gfx705``
     ``AMD:AMDGPU:8:0:0``  ``gfx802``
     ``AMD:AMDGPU:8:0:1``  ``gfx801:xnack+``
     ``AMD:AMDGPU:8:0:2``  ``gfx802``
     ``AMD:AMDGPU:8:0:3``  ``gfx803``
     ``AMD:AMDGPU:8:0:4``  ``gfx803``
     ``AMD:AMDGPU:8:0:5``  ``gfx805``
     ``AMD:AMDGPU:8:1:0``  ``gfx810:xnack+``
     ``AMD:AMDGPU:9:0:0``  ``gfx900:xnack-``
     ``AMD:AMDGPU:9:0:1``  ``gfx900:xnack+``
     ``AMD:AMDGPU:9:0:2``  ``gfx902:xnack-``
     ``AMD:AMDGPU:9:0:3``  ``gfx902:xnack+``
     ``AMD:AMDGPU:9:0:4``  ``gfx904:xnack-``
     ``AMD:AMDGPU:9:0:5``  ``gfx904:xnack+``
     ``AMD:AMDGPU:9:0:6``  ``gfx906:sramecc-:xnack-``
     ``AMD:AMDGPU:9:0:7``  ``gfx906:sramecc-:xnack+``
     ``AMD:AMDGPU:9:0:12`` ``gfx90c:xnack-``
     ===================== ==========================

.. _amdgpu-note-records-v3-onwards:

Code Object V3 and Above Note Records
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The AMDGPU backend code object uses the following ELF note record in the
``.note`` section when compiling for code object V3 and above.

The note record vendor field is "AMDGPU".

Additional note records may be present, but any which are not documented here
are deprecated and should not be used.

  .. table:: AMDGPU Code Object V3 and Above ELF Note Records
     :name: amdgpu-elf-note-records-table-v3-onwards

     ======== ============================== ======================================
     Name     Type                           Description
     ======== ============================== ======================================
     "AMDGPU" ``NT_AMDGPU_METADATA``         Metadata in Message Pack [MsgPack]_
                                             binary format.
     ======== ============================== ======================================

..

  .. table:: AMDGPU Code Object V3 and Above ELF Note Record Enumeration Values
     :name: amdgpu-elf-note-record-enumeration-values-table-v3-onwards

     ============================== =====
     Name                           Value
     ============================== =====
     *reserved*                     0-31
     ``NT_AMDGPU_METADATA``         32
     ============================== =====

``NT_AMDGPU_METADATA``
  Specifies extensible metadata associated with an AMDGPU code object. It is
  encoded as a map in the Message Pack [MsgPack]_ binary data format. See
  :ref:`amdgpu-amdhsa-code-object-metadata-v3`,
  :ref:`amdgpu-amdhsa-code-object-metadata-v4` and
  :ref:`amdgpu-amdhsa-code-object-metadata-v5` for the map keys defined for the
  ``amdhsa`` OS.

.. _amdgpu-symbols:

Symbols
-------

Symbols include the following:

  .. table:: AMDGPU ELF Symbols
     :name: amdgpu-elf-symbols-table

     ===================== ================== ================ ==================
     Name                  Type               Section          Description
     ===================== ================== ================ ==================
     *link-name*           ``STT_OBJECT``     - ``.data``      Global variable
                                              - ``.rodata``
                                              - ``.bss``
     *link-name*\ ``.kd``  ``STT_OBJECT``     - ``.rodata``    Kernel descriptor
     *link-name*           ``STT_FUNC``       - ``.text``      Kernel entry point
     *link-name*           ``STT_OBJECT``     - SHN_AMDGPU_LDS Global variable in LDS
     ===================== ================== ================ ==================

Global variable
  Global variables both used and defined by the compilation unit.

  If the symbol is defined in the compilation unit then it is allocated in the
  appropriate section according to if it has initialized data or is readonly.

  If the symbol is external then its section is ``STN_UNDEF`` and the loader
  will resolve relocations using the definition provided by another code object
  or explicitly defined by the runtime.

  If the symbol resides in local/group memory (LDS) then its section is the
  special processor specific section name ``SHN_AMDGPU_LDS``, and the
  ``st_value`` field describes alignment requirements as it does for common
  symbols.

  .. TODO::

     Add description of linked shared object symbols. Seems undefined symbols
     are marked as STT_NOTYPE.

Kernel descriptor
  Every HSA kernel has an associated kernel descriptor. It is the address of the
  kernel descriptor that is used in the AQL dispatch packet used to invoke the
  kernel, not the kernel entry point. The layout of the HSA kernel descriptor is
  defined in :ref:`amdgpu-amdhsa-kernel-descriptor`.

Kernel entry point
  Every HSA kernel also has a symbol for its machine code entry point.

.. _amdgpu-relocation-records:

Relocation Records
------------------

AMDGPU backend generates ``Elf64_Rela`` relocation records. Supported
relocatable fields are:

``word32``
  This specifies a 32-bit field occupying 4 bytes with arbitrary byte
  alignment. These values use the same byte order as other word values in the
  AMDGPU architecture.

``word64``
  This specifies a 64-bit field occupying 8 bytes with arbitrary byte
  alignment. These values use the same byte order as other word values in the
  AMDGPU architecture.

Following notations are used for specifying relocation calculations:

**A**
  Represents the addend used to compute the value of the relocatable field.

**G**
  Represents the offset into the global offset table at which the relocation
  entry's symbol will reside during execution.

**GOT**
  Represents the address of the global offset table.

**P**
  Represents the place (section offset for ``et_rel`` or address for ``et_dyn``)
  of the storage unit being relocated (computed using ``r_offset``).

**S**
  Represents the value of the symbol whose index resides in the relocation
  entry. Relocations not using this must specify a symbol index of
  ``STN_UNDEF``.

**B**
  Represents the base address of a loaded executable or shared object which is
  the difference between the ELF address and the actual load address.
  Relocations using this are only valid in executable or shared objects.

The following relocation types are supported:

  .. table:: AMDGPU ELF Relocation Records
     :name: amdgpu-elf-relocation-records-table

     ========================== ======= =====  ==========  ==============================
     Relocation Type            Kind    Value  Field       Calculation
     ========================== ======= =====  ==========  ==============================
     ``R_AMDGPU_NONE``                  0      *none*      *none*
     ``R_AMDGPU_ABS32_LO``      Static, 1      ``word32``  (S + A) & 0xFFFFFFFF
                                Dynamic
     ``R_AMDGPU_ABS32_HI``      Static, 2      ``word32``  (S + A) >> 32
                                Dynamic
     ``R_AMDGPU_ABS64``         Static, 3      ``word64``  S + A
                                Dynamic
     ``R_AMDGPU_REL32``         Static  4      ``word32``  S + A - P
     ``R_AMDGPU_REL64``         Static  5      ``word64``  S + A - P
     ``R_AMDGPU_ABS32``         Static, 6      ``word32``  S + A
                                Dynamic
     ``R_AMDGPU_GOTPCREL``      Static  7      ``word32``  G + GOT + A - P
     ``R_AMDGPU_GOTPCREL32_LO`` Static  8      ``word32``  (G + GOT + A - P) & 0xFFFFFFFF
     ``R_AMDGPU_GOTPCREL32_HI`` Static  9      ``word32``  (G + GOT + A - P) >> 32
     ``R_AMDGPU_REL32_LO``      Static  10     ``word32``  (S + A - P) & 0xFFFFFFFF
     ``R_AMDGPU_REL32_HI``      Static  11     ``word32``  (S + A - P) >> 32
     *reserved*                         12
     ``R_AMDGPU_RELATIVE64``    Dynamic 13     ``word64``  B + A
     ``R_AMDGPU_REL16``         Static  14     ``word16``  ((S + A - P) - 4) / 4
     ========================== ======= =====  ==========  ==============================

``R_AMDGPU_ABS32_LO`` and ``R_AMDGPU_ABS32_HI`` are only supported by
the ``mesa3d`` OS, which does not support ``R_AMDGPU_ABS64``.

There is no current OS loader support for 32-bit programs and so
``R_AMDGPU_ABS32`` is not used.

.. _amdgpu-loaded-code-object-path-uniform-resource-identifier:

Loaded Code Object Path Uniform Resource Identifier (URI)
---------------------------------------------------------

The AMD GPU code object loader represents the path of the ELF shared object from
which the code object was loaded as a textual Uniform Resource Identifier (URI).
Note that the code object is the in memory loaded relocated form of the ELF
shared object.  Multiple code objects may be loaded at different memory
addresses in the same process from the same ELF shared object.

The loaded code object path URI syntax is defined by the following BNF syntax:

.. code::

  code_object_uri ::== file_uri | memory_uri
  file_uri        ::== "file://" file_path [ range_specifier ]
  memory_uri      ::== "memory://" process_id range_specifier
  range_specifier ::== [ "#" | "?" ] "offset=" number "&" "size=" number
  file_path       ::== URI_ENCODED_OS_FILE_PATH
  process_id      ::== DECIMAL_NUMBER
  number          ::== HEX_NUMBER | DECIMAL_NUMBER | OCTAL_NUMBER

**number**
  Is a C integral literal where hexadecimal values are prefixed by "0x" or "0X",
  and octal values by "0".

**file_path**
  Is the file's path specified as a URI encoded UTF-8 string. In URI encoding,
  every character that is not in the regular expression ``[a-zA-Z0-9/_.~-]`` is
  encoded as two uppercase hexadecimal digits proceeded by "%".  Directories in
  the path are separated by "/".

**offset**
  Is a 0-based byte offset to the start of the code object.  For a file URI, it
  is from the start of the file specified by the ``file_path``, and if omitted
  defaults to 0. For a memory URI, it is the memory address and is required.

**size**
  Is the number of bytes in the code object.  For a file URI, if omitted it
  defaults to the size of the file.  It is required for a memory URI.

**process_id**
  Is the identity of the process owning the memory.  For Linux it is the C
  unsigned integral decimal literal for the process ID (PID).

For example:

.. code::

  file:///dir1/dir2/file1
  file:///dir3/dir4/file2#offset=0x2000&size=3000
  memory://1234#offset=0x20000&size=3000

.. _amdgpu-dwarf-debug-information:

DWARF Debug Information
=======================

.. warning::

   This section describes **provisional support** for AMDGPU DWARF [DWARF]_ that
   is not currently fully implemented and is subject to change.

AMDGPU generates DWARF [DWARF]_ debugging information ELF sections (see
:ref:`amdgpu-elf-code-object`) which contain information that maps the code
object executable code and data to the source language constructs. It can be
used by tools such as debuggers and profilers. It uses features defined in
:doc:`AMDGPUDwarfExtensionsForHeterogeneousDebugging` that are made available in
DWARF Version 4 and DWARF Version 5 as an LLVM vendor extension.

This section defines the AMDGPU target architecture specific DWARF mappings.

.. _amdgpu-dwarf-register-identifier:

Register Identifier
-------------------

This section defines the AMDGPU target architecture register numbers used in
DWARF operation expressions (see DWARF Version 5 section 2.5 and
:ref:`amdgpu-dwarf-operation-expressions`) and Call Frame Information
instructions (see DWARF Version 5 section 6.4 and
:ref:`amdgpu-dwarf-call-frame-information`).

A single code object can contain code for kernels that have different wavefront
sizes. The vector registers and some scalar registers are based on the wavefront
size. AMDGPU defines distinct DWARF registers for each wavefront size. This
simplifies the consumer of the DWARF so that each register has a fixed size,
rather than being dynamic according to the wavefront size mode. Similarly,
distinct DWARF registers are defined for those registers that vary in size
according to the process address size. This allows a consumer to treat a
specific AMDGPU processor as a single architecture regardless of how it is
configured at run time. The compiler explicitly specifies the DWARF registers
that match the mode in which the code it is generating will be executed.

DWARF registers are encoded as numbers, which are mapped to architecture
registers. The mapping for AMDGPU is defined in
:ref:`amdgpu-dwarf-register-mapping-table`. All AMDGPU targets use the same
mapping.

.. table:: AMDGPU DWARF Register Mapping
   :name: amdgpu-dwarf-register-mapping-table

   ============== ================= ======== ==================================
   DWARF Register AMDGPU Register   Bit Size Description
   ============== ================= ======== ==================================
   0              PC_32             32       Program Counter (PC) when
                                             executing in a 32-bit process
                                             address space. Used in the CFI to
                                             describe the PC of the calling
                                             frame.
   1              EXEC_MASK_32      32       Execution Mask Register when
                                             executing in wavefront 32 mode.
   2-15           *Reserved*                 *Reserved for highly accessed
                                             registers using DWARF shortcut.*
   16             PC_64             64       Program Counter (PC) when
                                             executing in a 64-bit process
                                             address space. Used in the CFI to
                                             describe the PC of the calling
                                             frame.
   17             EXEC_MASK_64      64       Execution Mask Register when
                                             executing in wavefront 64 mode.
   18-31          *Reserved*                 *Reserved for highly accessed
                                             registers using DWARF shortcut.*
   32-95          SGPR0-SGPR63      32       Scalar General Purpose
                                             Registers.
   96-127         *Reserved*                 *Reserved for frequently accessed
                                             registers using DWARF 1-byte ULEB.*
   128            STATUS            32       Status Register.
   129-511        *Reserved*                 *Reserved for future Scalar
                                             Architectural Registers.*
   512            VCC_32            32       Vector Condition Code Register
                                             when executing in wavefront 32
                                             mode.
   513-767        *Reserved*                 *Reserved for future Vector
                                             Architectural Registers when
                                             executing in wavefront 32 mode.*
   768            VCC_64            64       Vector Condition Code Register
                                             when executing in wavefront 64
                                             mode.
   769-1023       *Reserved*                 *Reserved for future Vector
                                             Architectural Registers when
                                             executing in wavefront 64 mode.*
   1024-1087      *Reserved*                 *Reserved for padding.*
   1088-1129      SGPR64-SGPR105    32       Scalar General Purpose Registers.
   1130-1535      *Reserved*                 *Reserved for future Scalar
                                             General Purpose Registers.*
   1536-1791      VGPR0-VGPR255     32*32    Vector General Purpose Registers
                                             when executing in wavefront 32
                                             mode.
   1792-2047      *Reserved*                 *Reserved for future Vector
                                             General Purpose Registers when
                                             executing in wavefront 32 mode.*
   2048-2303      AGPR0-AGPR255     32*32    Vector Accumulation Registers
                                             when executing in wavefront 32
                                             mode.
   2304-2559      *Reserved*                 *Reserved for future Vector
                                             Accumulation Registers when
                                             executing in wavefront 32 mode.*
   2560-2815      VGPR0-VGPR255     64*32    Vector General Purpose Registers
                                             when executing in wavefront 64
                                             mode.
   2816-3071      *Reserved*                 *Reserved for future Vector
                                             General Purpose Registers when
                                             executing in wavefront 64 mode.*
   3072-3327      AGPR0-AGPR255     64*32    Vector Accumulation Registers
                                             when executing in wavefront 64
                                             mode.
   3328-3583      *Reserved*                 *Reserved for future Vector
                                             Accumulation Registers when
                                             executing in wavefront 64 mode.*
   ============== ================= ======== ==================================

The vector registers are represented as the full size for the wavefront. They
are organized as consecutive dwords (32-bits), one per lane, with the dword at
the least significant bit position corresponding to lane 0 and so forth. DWARF
location expressions involving the ``DW_OP_LLVM_offset`` and
``DW_OP_LLVM_push_lane`` operations are used to select the part of the vector
register corresponding to the lane that is executing the current thread of
execution in languages that are implemented using a SIMD or SIMT execution
model.

If the wavefront size is 32 lanes then the wavefront 32 mode register
definitions are used. If the wavefront size is 64 lanes then the wavefront 64
mode register definitions are used. Some AMDGPU targets support executing in
both wavefront 32 and wavefront 64 mode. The register definitions corresponding
to the wavefront mode of the generated code will be used.

If code is generated to execute in a 32-bit process address space, then the
32-bit process address space register definitions are used. If code is generated
to execute in a 64-bit process address space, then the 64-bit process address
space register definitions are used. The ``amdgcn`` target only supports the
64-bit process address space.

.. _amdgpu-dwarf-memory-space-identifier:

Memory Space Identifier
-----------------------

The DWARF memory space represents the source language memory space. See DWARF
Version 5 section 2.12 which is updated by the *DWARF Extensions For
Heterogeneous Debugging* section :ref:`amdgpu-dwarf-memory-spaces`.

The DWARF memory space mapping used for AMDGPU is defined in
:ref:`amdgpu-dwarf-memory-space-mapping-table`.

.. table:: AMDGPU DWARF Memory Space Mapping
   :name: amdgpu-dwarf-memory-space-mapping-table

   =========================== ====== =================
   DWARF                              AMDGPU
   ---------------------------------- -----------------
   Memory Space Name           Value  Memory Space
   =========================== ====== =================
   ``DW_MSPACE_LLVM_none``     0x0000 Generic (Flat)
   ``DW_MSPACE_LLVM_global``   0x0001 Global
   ``DW_MSPACE_LLVM_constant`` 0x0002 Global
   ``DW_MSPACE_LLVM_group``    0x0003 Local (group/LDS)
   ``DW_MSPACE_LLVM_private``  0x0004 Private (Scratch)
   ``DW_MSPACE_AMDGPU_region`` 0x8000 Region (GDS)
   =========================== ====== =================

The DWARF memory space values defined in the *DWARF Extensions For Heterogeneous
Debugging* section :ref:`amdgpu-dwarf-memory-spaces` are used.

In addition, ``DW_ADDR_AMDGPU_region`` is encoded as a vendor extension. This is
available for use for the AMD extension for access to the hardware GDS memory
which is scratchpad memory allocated per device.

For AMDGPU if no ``DW_AT_LLVM_memory_space`` attribute is present, then the
default memory space of ``DW_MSPACE_LLVM_none`` is used.

See :ref:`amdgpu-dwarf-address-space-identifier` for information on the AMDGPU
mapping of DWARF memory spaces to DWARF address spaces, including address size
and NULL value.

.. _amdgpu-dwarf-address-space-identifier:

Address Space Identifier
------------------------

DWARF address spaces correspond to target architecture specific linear
addressable memory areas. See DWARF Version 5 section 2.12 and *DWARF Extensions
For Heterogeneous Debugging* section :ref:`amdgpu-dwarf-address-spaces`.

The DWARF address space mapping used for AMDGPU is defined in
:ref:`amdgpu-dwarf-address-space-mapping-table`.

.. table:: AMDGPU DWARF Address Space Mapping
   :name: amdgpu-dwarf-address-space-mapping-table

   ======================================= ===== ======= ======== ===================== =======================
   DWARF                                                          AMDGPU                Notes
   --------------------------------------- ----- ---------------- --------------------- -----------------------
   Address Space Name                      Value Address Bit Size LLVM IR Address Space
   --------------------------------------- ----- ------- -------- --------------------- -----------------------
   ..                                            64-bit  32-bit
                                                 process process
                                                 address address
                                                 space   space
   ======================================= ===== ======= ======== ===================== =======================
   ``DW_ASPACE_LLVM_none``                 0x00  64      32       Global                *default address space*
   ``DW_ASPACE_AMDGPU_generic``            0x01  64      32       Generic (Flat)
   ``DW_ASPACE_AMDGPU_region``             0x02  32      32       Region (GDS)
   ``DW_ASPACE_AMDGPU_local``              0x03  32      32       Local (group/LDS)
   *Reserved*                              0x04
   ``DW_ASPACE_AMDGPU_private_lane``       0x05  32      32       Private (Scratch)     *focused lane*
   ``DW_ASPACE_AMDGPU_private_wave``       0x06  32      32       Private (Scratch)     *unswizzled wavefront*
   ======================================= ===== ======= ======== ===================== =======================

See :ref:`amdgpu-address-spaces` for information on the AMDGPU LLVM IR address
spaces including address size and NULL value.

The ``DW_ASPACE_LLVM_none`` address space is the default target architecture
address space used in DWARF operations that do not specify an address space. It
therefore has to map to the global address space so that the ``DW_OP_addr*`` and
related operations can refer to addresses in the program code.

The ``DW_ASPACE_AMDGPU_generic`` address space allows location expressions to
specify the flat address space. If the address corresponds to an address in the
local address space, then it corresponds to the wavefront that is executing the
focused thread of execution. If the address corresponds to an address in the
private address space, then it corresponds to the lane that is executing the
focused thread of execution for languages that are implemented using a SIMD or
SIMT execution model.

.. note::

  CUDA-like languages such as HIP that do not have address spaces in the
  language type system, but do allow variables to be allocated in different
  address spaces, need to explicitly specify the ``DW_ASPACE_AMDGPU_generic``
  address space in the DWARF expression operations as the default address space
  is the global address space.

The ``DW_ASPACE_AMDGPU_local`` address space allows location expressions to
specify the local address space corresponding to the wavefront that is executing
the focused thread of execution.

The ``DW_ASPACE_AMDGPU_private_lane`` address space allows location expressions
to specify the private address space corresponding to the lane that is executing
the focused thread of execution for languages that are implemented using a SIMD
or SIMT execution model.

The ``DW_ASPACE_AMDGPU_private_wave`` address space allows location expressions
to specify the unswizzled private address space corresponding to the wavefront
that is executing the focused thread of execution. The wavefront view of private
memory is the per wavefront unswizzled backing memory layout defined in
:ref:`amdgpu-address-spaces`, such that address 0 corresponds to the first
location for the backing memory of the wavefront (namely the address is not
offset by ``wavefront-scratch-base``). The following formula can be used to
convert from a ``DW_ASPACE_AMDGPU_private_lane`` address to a
``DW_ASPACE_AMDGPU_private_wave`` address:

::

  private-address-wavefront =
    ((private-address-lane / 4) * wavefront-size * 4) +
    (wavefront-lane-id * 4) + (private-address-lane % 4)

If the ``DW_ASPACE_AMDGPU_private_lane`` address is dword aligned, and the start
of the dwords for each lane starting with lane 0 is required, then this
simplifies to:

::

  private-address-wavefront =
    private-address-lane * wavefront-size

A compiler can use the ``DW_ASPACE_AMDGPU_private_wave`` address space to read a
complete spilled vector register back into a complete vector register in the
CFI. The frame pointer can be a private lane address which is dword aligned,
which can be shifted to multiply by the wavefront size, and then used to form a
private wavefront address that gives a location for a contiguous set of dwords,
one per lane, where the vector register dwords are spilled. The compiler knows
the wavefront size since it generates the code. Note that the type of the
address may have to be converted as the size of a
``DW_ASPACE_AMDGPU_private_lane`` address may be smaller than the size of a
``DW_ASPACE_AMDGPU_private_wave`` address.

.. _amdgpu-dwarf-lane-identifier:

Lane identifier
---------------

DWARF lane identifies specify a target architecture lane position for hardware
that executes in a SIMD or SIMT manner, and on which a source language maps its
threads of execution onto those lanes. The DWARF lane identifier is pushed by
the ``DW_OP_LLVM_push_lane`` DWARF expression operation. See DWARF Version 5
section 2.5 which is updated by *DWARF Extensions For Heterogeneous Debugging*
section :ref:`amdgpu-dwarf-operation-expressions`.

For AMDGPU, the lane identifier corresponds to the hardware lane ID of a
wavefront. It is numbered from 0 to the wavefront size minus 1.

Operation Expressions
---------------------

DWARF expressions are used to compute program values and the locations of
program objects. See DWARF Version 5 section 2.5 and
:ref:`amdgpu-dwarf-operation-expressions`.

DWARF location descriptions describe how to access storage which includes memory
and registers. When accessing storage on AMDGPU, bytes are ordered with least
significant bytes first, and bits are ordered within bytes with least
significant bits first.

For AMDGPU CFI expressions, ``DW_OP_LLVM_select_bit_piece`` is used to describe
unwinding vector registers that are spilled under the execution mask to memory:
the zero-single location description is the vector register, and the one-single
location description is the spilled memory location description. The
``DW_OP_LLVM_form_aspace_address`` is used to specify the address space of the
memory location description.

In AMDGPU expressions, ``DW_OP_LLVM_select_bit_piece`` is used by the
``DW_AT_LLVM_lane_pc`` attribute expression where divergent control flow is
controlled by the execution mask. An undefined location description together
with ``DW_OP_LLVM_extend`` is used to indicate the lane was not active on entry
to the subprogram. See :ref:`amdgpu-dwarf-dw-at-llvm-lane-pc` for an example.

Debugger Information Entry Attributes
-------------------------------------

This section describes how certain debugger information entry attributes are
used by AMDGPU. See the sections in DWARF Version 5 section 3.3.5 and 3.1.1
which are updated by *DWARF Extensions For Heterogeneous Debugging* section
:ref:`amdgpu-dwarf-low-level-information` and
:ref:`amdgpu-dwarf-full-and-partial-compilation-unit-entries`.

.. _amdgpu-dwarf-dw-at-llvm-lane-pc:

``DW_AT_LLVM_lane_pc``
~~~~~~~~~~~~~~~~~~~~~~

For AMDGPU, the ``DW_AT_LLVM_lane_pc`` attribute is used to specify the program
location of the separate lanes of a SIMT thread.

If the lane is an active lane then this will be the same as the current program
location.

If the lane is inactive, but was active on entry to the subprogram, then this is
the program location in the subprogram at which execution of the lane is
conceptual positioned.

If the lane was not active on entry to the subprogram, then this will be the
undefined location. A client debugger can check if the lane is part of a valid
work-group by checking that the lane is in the range of the associated
work-group within the grid, accounting for partial work-groups. If it is not,
then the debugger can omit any information for the lane. Otherwise, the debugger
may repeatedly unwind the stack and inspect the ``DW_AT_LLVM_lane_pc`` of the
calling subprogram until it finds a non-undefined location. Conceptually the
lane only has the call frames that it has a non-undefined
``DW_AT_LLVM_lane_pc``.

The following example illustrates how the AMDGPU backend can generate a DWARF
location list expression for the nested ``IF/THEN/ELSE`` structures of the
following subprogram pseudo code for a target with 64 lanes per wavefront.

.. code::
  :number-lines:

  SUBPROGRAM X
  BEGIN
    a;
    IF (c1) THEN
      b;
      IF (c2) THEN
        c;
      ELSE
        d;
      ENDIF
      e;
    ELSE
      f;
    ENDIF
    g;
  END

The AMDGPU backend may generate the following pseudo LLVM MIR to manipulate the
execution mask (``EXEC``) to linearize the control flow. The condition is
evaluated to make a mask of the lanes for which the condition evaluates to true.
First the ``THEN`` region is executed by setting the ``EXEC`` mask to the
logical ``AND`` of the current ``EXEC`` mask with the condition mask. Then the
``ELSE`` region is executed by negating the ``EXEC`` mask and logical ``AND`` of
the saved ``EXEC`` mask at the start of the region. After the ``IF/THEN/ELSE``
region the ``EXEC`` mask is restored to the value it had at the beginning of the
region. This is shown below. Other approaches are possible, but the basic
concept is the same.

.. code::
  :number-lines:

  $lex_start:
    a;
    %1 = EXEC
    %2 = c1
  $lex_1_start:
    EXEC = %1 & %2
  $if_1_then:
      b;
      %3 = EXEC
      %4 = c2
  $lex_1_1_start:
      EXEC = %3 & %4
  $lex_1_1_then:
        c;
      EXEC = ~EXEC & %3
  $lex_1_1_else:
        d;
      EXEC = %3
  $lex_1_1_end:
      e;
    EXEC = ~EXEC & %1
  $lex_1_else:
      f;
    EXEC = %1
  $lex_1_end:
    g;
  $lex_end:

To create the DWARF location list expression that defines the location
description of a vector of lane program locations, the LLVM MIR ``DBG_VALUE``
pseudo instruction can be used to annotate the linearized control flow. This can
be done by defining an artificial variable for the lane PC. The DWARF location
list expression created for it is used as the value of the
``DW_AT_LLVM_lane_pc`` attribute on the subprogram's debugger information entry.

A DWARF procedure is defined for each well nested structured control flow region
which provides the conceptual lane program location for a lane if it is not
active (namely it is divergent). The DWARF operation expression for each region
conceptually inherits the value of the immediately enclosing region and modifies
it according to the semantics of the region.

For an ``IF/THEN/ELSE`` region the divergent program location is at the start of
the region for the ``THEN`` region since it is executed first. For the ``ELSE``
region the divergent program location is at the end of the ``IF/THEN/ELSE``
region since the ``THEN`` region has completed.

The lane PC artificial variable is assigned at each region transition. It uses
the immediately enclosing region's DWARF procedure to compute the program
location for each lane assuming they are divergent, and then modifies the result
by inserting the current program location for each lane that the ``EXEC`` mask
indicates is active.

By having separate DWARF procedures for each region, they can be reused to
define the value for any nested region. This reduces the total size of the DWARF
operation expressions.

The following provides an example using pseudo LLVM MIR.

.. code::
  :number-lines:

  $lex_start:
    DEFINE_DWARF %__uint_64 = DW_TAG_base_type[
      DW_AT_name = "__uint64";
      DW_AT_byte_size = 8;
      DW_AT_encoding = DW_ATE_unsigned;
    ];
    DEFINE_DWARF %__active_lane_pc = DW_TAG_dwarf_procedure[
      DW_AT_name = "__active_lane_pc";
      DW_AT_location = [
        DW_OP_regx PC;
        DW_OP_LLVM_extend 64, 64;
        DW_OP_regval_type EXEC, %uint_64;
        DW_OP_LLVM_select_bit_piece 64, 64;
      ];
    ];
    DEFINE_DWARF %__divergent_lane_pc = DW_TAG_dwarf_procedure[
      DW_AT_name = "__divergent_lane_pc";
      DW_AT_location = [
        DW_OP_LLVM_undefined;
        DW_OP_LLVM_extend 64, 64;
      ];
    ];
    DBG_VALUE $noreg, $noreg, %DW_AT_LLVM_lane_pc, DIExpression[
      DW_OP_call_ref %__divergent_lane_pc;
      DW_OP_call_ref %__active_lane_pc;
    ];
    a;
    %1 = EXEC;
    DBG_VALUE %1, $noreg, %__lex_1_save_exec;
    %2 = c1;
  $lex_1_start:
    EXEC = %1 & %2;
  $lex_1_then:
      DEFINE_DWARF %__divergent_lane_pc_1_then = DW_TAG_dwarf_procedure[
        DW_AT_name = "__divergent_lane_pc_1_then";
        DW_AT_location = DIExpression[
          DW_OP_call_ref %__divergent_lane_pc;
          DW_OP_addrx &lex_1_start;
          DW_OP_stack_value;
          DW_OP_LLVM_extend 64, 64;
          DW_OP_call_ref %__lex_1_save_exec;
          DW_OP_deref_type 64, %__uint_64;
          DW_OP_LLVM_select_bit_piece 64, 64;
        ];
      ];
      DBG_VALUE $noreg, $noreg, %DW_AT_LLVM_lane_pc, DIExpression[
        DW_OP_call_ref %__divergent_lane_pc_1_then;
        DW_OP_call_ref %__active_lane_pc;
      ];
      b;
      %3 = EXEC;
      DBG_VALUE %3, %__lex_1_1_save_exec;
      %4 = c2;
  $lex_1_1_start:
      EXEC = %3 & %4;
  $lex_1_1_then:
        DEFINE_DWARF %__divergent_lane_pc_1_1_then = DW_TAG_dwarf_procedure[
          DW_AT_name = "__divergent_lane_pc_1_1_then";
          DW_AT_location = DIExpression[
            DW_OP_call_ref %__divergent_lane_pc_1_then;
            DW_OP_addrx &lex_1_1_start;
            DW_OP_stack_value;
            DW_OP_LLVM_extend 64, 64;
            DW_OP_call_ref %__lex_1_1_save_exec;
            DW_OP_deref_type 64, %__uint_64;
            DW_OP_LLVM_select_bit_piece 64, 64;
          ];
        ];
        DBG_VALUE $noreg, $noreg, %DW_AT_LLVM_lane_pc, DIExpression[
          DW_OP_call_ref %__divergent_lane_pc_1_1_then;
          DW_OP_call_ref %__active_lane_pc;
        ];
        c;
      EXEC = ~EXEC & %3;
  $lex_1_1_else:
        DEFINE_DWARF %__divergent_lane_pc_1_1_else = DW_TAG_dwarf_procedure[
          DW_AT_name = "__divergent_lane_pc_1_1_else";
          DW_AT_location = DIExpression[
            DW_OP_call_ref %__divergent_lane_pc_1_then;
            DW_OP_addrx &lex_1_1_end;
            DW_OP_stack_value;
            DW_OP_LLVM_extend 64, 64;
            DW_OP_call_ref %__lex_1_1_save_exec;
            DW_OP_deref_type 64, %__uint_64;
            DW_OP_LLVM_select_bit_piece 64, 64;
          ];
        ];
        DBG_VALUE $noreg, $noreg, %DW_AT_LLVM_lane_pc, DIExpression[
          DW_OP_call_ref %__divergent_lane_pc_1_1_else;
          DW_OP_call_ref %__active_lane_pc;
        ];
        d;
      EXEC = %3;
  $lex_1_1_end:
      DBG_VALUE $noreg, $noreg, %DW_AT_LLVM_lane_pc, DIExpression[
        DW_OP_call_ref %__divergent_lane_pc;
        DW_OP_call_ref %__active_lane_pc;
      ];
      e;
    EXEC = ~EXEC & %1;
  $lex_1_else:
      DEFINE_DWARF %__divergent_lane_pc_1_else = DW_TAG_dwarf_procedure[
        DW_AT_name = "__divergent_lane_pc_1_else";
        DW_AT_location = DIExpression[
          DW_OP_call_ref %__divergent_lane_pc;
          DW_OP_addrx &lex_1_end;
          DW_OP_stack_value;
          DW_OP_LLVM_extend 64, 64;
          DW_OP_call_ref %__lex_1_save_exec;
          DW_OP_deref_type 64, %__uint_64;
          DW_OP_LLVM_select_bit_piece 64, 64;
        ];
      ];
      DBG_VALUE $noreg, $noreg, %DW_AT_LLVM_lane_pc, DIExpression[
        DW_OP_call_ref %__divergent_lane_pc_1_else;
        DW_OP_call_ref %__active_lane_pc;
      ];
      f;
    EXEC = %1;
  $lex_1_end:
    DBG_VALUE $noreg, $noreg, %DW_AT_LLVM_lane_pc DIExpression[
      DW_OP_call_ref %__divergent_lane_pc;
      DW_OP_call_ref %__active_lane_pc;
    ];
    g;
  $lex_end:

The DWARF procedure ``%__active_lane_pc`` is used to update the lane pc elements
that are active, with the current program location.

Artificial variables %__lex_1_save_exec and %__lex_1_1_save_exec are created for
the execution masks saved on entry to a region. Using the ``DBG_VALUE`` pseudo
instruction, location list entries will be created that describe where the
artificial variables are allocated at any given program location. The compiler
may allocate them to registers or spill them to memory.

The DWARF procedures for each region use the values of the saved execution mask
artificial variables to only update the lanes that are active on entry to the
region. All other lanes retain the value of the enclosing region where they were
last active. If they were not active on entry to the subprogram, then will have
the undefined location description.

Other structured control flow regions can be handled similarly. For example,
loops would set the divergent program location for the region at the end of the
loop. Any lanes active will be in the loop, and any lanes not active must have
exited the loop.

An ``IF/THEN/ELSEIF/ELSEIF/...`` region can be treated as a nest of
``IF/THEN/ELSE`` regions.

The DWARF procedures can use the active lane artificial variable described in
:ref:`amdgpu-dwarf-amdgpu-dw-at-llvm-active-lane` rather than the actual
``EXEC`` mask in order to support whole or quad wavefront mode.

.. _amdgpu-dwarf-amdgpu-dw-at-llvm-active-lane:

``DW_AT_LLVM_active_lane``
~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``DW_AT_LLVM_active_lane`` attribute on a subprogram debugger information
entry is used to specify the lanes that are conceptually active for a SIMT
thread.

The execution mask may be modified to implement whole or quad wavefront mode
operations. For example, all lanes may need to temporarily be made active to
execute a whole wavefront operation. Such regions would save the ``EXEC`` mask,
update it to enable the necessary lanes, perform the operations, and then
restore the ``EXEC`` mask from the saved value. While executing the whole
wavefront region, the conceptual execution mask is the saved value, not the
``EXEC`` value.

This is handled by defining an artificial variable for the active lane mask. The
active lane mask artificial variable would be the actual ``EXEC`` mask for
normal regions, and the saved execution mask for regions where the mask is
temporarily updated. The location list expression created for this artificial
variable is used to define the value of the ``DW_AT_LLVM_active_lane``
attribute.

``DW_AT_LLVM_augmentation``
~~~~~~~~~~~~~~~~~~~~~~~~~~~

For AMDGPU, the ``DW_AT_LLVM_augmentation`` attribute of a compilation unit
debugger information entry has the following value for the augmentation string:

::

  [amdgpu:v0.0]

The "vX.Y" specifies the major X and minor Y version number of the AMDGPU
extensions used in the DWARF of the compilation unit. The version number
conforms to [SEMVER]_.

Call Frame Information
----------------------

DWARF Call Frame Information (CFI) describes how a consumer can virtually
*unwind* call frames in a running process or core dump. See DWARF Version 5
section 6.4 and :ref:`amdgpu-dwarf-call-frame-information`.

For AMDGPU, the Common Information Entry (CIE) fields have the following values:

1.  ``augmentation`` string contains the following null-terminated UTF-8 string:

    ::

      [amd:v0.0]

    The ``vX.Y`` specifies the major X and minor Y version number of the AMDGPU
    extensions used in this CIE or to the FDEs that use it. The version number
    conforms to [SEMVER]_.

2.  ``address_size`` for the ``Global`` address space is defined in
    :ref:`amdgpu-dwarf-address-space-identifier`.

3.  ``segment_selector_size`` is 0 as AMDGPU does not use a segment selector.

4.  ``code_alignment_factor`` is 4 bytes.

    .. TODO::

       Add to :ref:`amdgpu-processor-table` table.

5.  ``data_alignment_factor`` is 4 bytes.

    .. TODO::

       Add to :ref:`amdgpu-processor-table` table.

6.  ``return_address_register`` is ``PC_32`` for 32-bit processes and ``PC_64``
    for 64-bit processes defined in :ref:`amdgpu-dwarf-register-identifier`.

7.  ``initial_instructions`` Since a subprogram X with fewer registers can be
    called from subprogram Y that has more allocated, X will not change any of
    the extra registers as it cannot access them. Therefore, the default rule
    for all columns is ``same value``.

For AMDGPU the register number follows the numbering defined in
:ref:`amdgpu-dwarf-register-identifier`.

For AMDGPU the instructions are variable size. A consumer can subtract 1 from
the return address to get the address of a byte within the call site
instructions. See DWARF Version 5 section 6.4.4.

Accelerated Access
------------------

See DWARF Version 5 section 6.1.

Lookup By Name Section Header
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See DWARF Version 5 section 6.1.1.4.1 and :ref:`amdgpu-dwarf-lookup-by-name`.

For AMDGPU the lookup by name section header table:

``augmentation_string_size`` (uword)

  Set to the length of the ``augmentation_string`` value which is always a
  multiple of 4.

``augmentation_string`` (sequence of UTF-8 characters)

  Contains the following UTF-8 string null padded to a multiple of 4 bytes:

  ::

    [amdgpu:v0.0]

  The "vX.Y" specifies the major X and minor Y version number of the AMDGPU
  extensions used in the DWARF of this index. The version number conforms to
  [SEMVER]_.

  .. note::

    This is different to the DWARF Version 5 definition that requires the first
    4 characters to be the vendor ID. But this is consistent with the other
    augmentation strings and does allow multiple vendor contributions. However,
    backwards compatibility may be more desirable.

Lookup By Address Section Header
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See DWARF Version 5 section 6.1.2.

For AMDGPU the lookup by address section header table:

``address_size`` (ubyte)

  Match the address size for the ``Global`` address space defined in
  :ref:`amdgpu-dwarf-address-space-identifier`.

``segment_selector_size`` (ubyte)

  AMDGPU does not use a segment selector so this is 0. The entries in the
  ``.debug_aranges`` do not have a segment selector.

Line Number Information
-----------------------

See DWARF Version 5 section 6.2 and :ref:`amdgpu-dwarf-line-number-information`.

AMDGPU does not use the ``isa`` state machine registers and always sets it to 0.
The instruction set must be obtained from the ELF file header ``e_flags`` field
in the ``EF_AMDGPU_MACH`` bit position (see :ref:`ELF Header
<amdgpu-elf-header>`). See DWARF Version 5 section 6.2.2.

.. TODO::

  Should the ``isa`` state machine register be used to indicate if the code is
  in wavefront32 or wavefront64 mode? Or used to specify the architecture ISA?

For AMDGPU the line number program header fields have the following values (see
DWARF Version 5 section 6.2.4):

``address_size`` (ubyte)
  Matches the address size for the ``Global`` address space defined in
  :ref:`amdgpu-dwarf-address-space-identifier`.

``segment_selector_size`` (ubyte)
  AMDGPU does not use a segment selector so this is 0.

``minimum_instruction_length`` (ubyte)
  For GFX9-GFX11 this is 4.

``maximum_operations_per_instruction`` (ubyte)
  For GFX9-GFX11 this is 1.

Source text for online-compiled programs (for example, those compiled by the
OpenCL language runtime) may be embedded into the DWARF Version 5 line table.
See DWARF Version 5 section 6.2.4.1 which is updated by *DWARF Extensions For
Heterogeneous Debugging* section :ref:`DW_LNCT_LLVM_source
<amdgpu-dwarf-line-number-information-dw-lnct-llvm-source>`.

The Clang option used to control source embedding in AMDGPU is defined in
:ref:`amdgpu-clang-debug-options-table`.

  .. table:: AMDGPU Clang Debug Options
     :name: amdgpu-clang-debug-options-table

     ==================== ==================================================
     Debug Flag           Description
     ==================== ==================================================
     -g[no-]embed-source  Enable/disable embedding source text in DWARF
                          debug sections. Useful for environments where
                          source cannot be written to disk, such as
                          when performing online compilation.
     ==================== ==================================================

For example:

``-gembed-source``
  Enable the embedded source.

``-gno-embed-source``
  Disable the embedded source.

32-Bit and 64-Bit DWARF Formats
-------------------------------

See DWARF Version 5 section 7.4 and
:ref:`amdgpu-dwarf-32-bit-and-64-bit-dwarf-formats`.

For AMDGPU:

* For the ``amdgcn`` target architecture only the 64-bit process address space
  is supported.

* The producer can generate either 32-bit or 64-bit DWARF format. LLVM generates
  the 32-bit DWARF format.

Unit Headers
------------

For AMDGPU the following values apply for each of the unit headers described in
DWARF Version 5 sections 7.5.1.1, 7.5.1.2, and 7.5.1.3:

``address_size`` (ubyte)
  Matches the address size for the ``Global`` address space defined in
  :ref:`amdgpu-dwarf-address-space-identifier`.

.. _amdgpu-code-conventions:

Code Conventions
================

This section provides code conventions used for each supported target triple OS
(see :ref:`amdgpu-target-triples`).

AMDHSA
------

This section provides code conventions used when the target triple OS is
``amdhsa`` (see :ref:`amdgpu-target-triples`).

.. _amdgpu-amdhsa-code-object-metadata:

Code Object Metadata
~~~~~~~~~~~~~~~~~~~~

The code object metadata specifies extensible metadata associated with the code
objects executed on HSA [HSA]_ compatible runtimes (see :ref:`amdgpu-os`). The
encoding and semantics of this metadata depends on the code object version; see
:ref:`amdgpu-amdhsa-code-object-metadata-v2`,
:ref:`amdgpu-amdhsa-code-object-metadata-v3`,
:ref:`amdgpu-amdhsa-code-object-metadata-v4` and
:ref:`amdgpu-amdhsa-code-object-metadata-v5`.

Code object metadata is specified in a note record (see
:ref:`amdgpu-note-records`) and is required when the target triple OS is
``amdhsa`` (see :ref:`amdgpu-target-triples`). It must contain the minimum
information necessary to support the HSA compatible runtime kernel queries. For
example, the segment sizes needed in a dispatch packet. In addition, a
high-level language runtime may require other information to be included. For
example, the AMD OpenCL runtime records kernel argument information.

.. _amdgpu-amdhsa-code-object-metadata-v2:

Code Object V2 Metadata
+++++++++++++++++++++++

.. warning::
  Code object V2 is not the default code object version emitted by this version
  of LLVM.

Code object V2 metadata is specified by the ``NT_AMD_HSA_METADATA`` note record
(see :ref:`amdgpu-note-records-v2`).

The metadata is specified as a YAML formatted string (see [YAML]_ and
:doc:`YamlIO`).

.. TODO::

  Is the string null terminated? It probably should not if YAML allows it to
  contain null characters, otherwise it should be.

The metadata is represented as a single YAML document comprised of the mapping
defined in table :ref:`amdgpu-amdhsa-code-object-metadata-map-v2-table` and
referenced tables.

For boolean values, the string values of ``false`` and ``true`` are used for
false and true respectively.

Additional information can be added to the mappings. To avoid conflicts, any
non-AMD key names should be prefixed by "*vendor-name*.".

  .. table:: AMDHSA Code Object V2 Metadata Map
     :name: amdgpu-amdhsa-code-object-metadata-map-v2-table

     ========== ============== ========= =======================================
     String Key Value Type     Required? Description
     ========== ============== ========= =======================================
     "Version"  sequence of    Required  - The first integer is the major
                2 integers                 version. Currently 1.
                                         - The second integer is the minor
                                           version. Currently 0.
     "Printf"   sequence of              Each string is encoded information
                strings                  about a printf function call. The
                                         encoded information is organized as
                                         fields separated by colon (':'):

                                         ``ID:N:S[0]:S[1]:...:S[N-1]:FormatString``

                                         where:

                                         ``ID``
                                           A 32-bit integer as a unique id for
                                           each printf function call

                                         ``N``
                                           A 32-bit integer equal to the number
                                           of arguments of printf function call
                                           minus 1

                                         ``S[i]`` (where i = 0, 1, ... , N-1)
                                           32-bit integers for the size in bytes
                                           of the i-th FormatString argument of
                                           the printf function call

                                         FormatString
                                           The format string passed to the
                                           printf function call.
     "Kernels"  sequence of    Required  Sequence of the mappings for each
                mapping                  kernel in the code object. See
                                         :ref:`amdgpu-amdhsa-code-object-kernel-metadata-map-v2-table`
                                         for the definition of the mapping.
     ========== ============== ========= =======================================

..

  .. table:: AMDHSA Code Object V2 Kernel Metadata Map
     :name: amdgpu-amdhsa-code-object-kernel-metadata-map-v2-table

     ================= ============== ========= ================================
     String Key        Value Type     Required? Description
     ================= ============== ========= ================================
     "Name"            string         Required  Source name of the kernel.
     "SymbolName"      string         Required  Name of the kernel
                                                descriptor ELF symbol.
     "Language"        string                   Source language of the kernel.
                                                Values include:

                                                - "OpenCL C"
                                                - "OpenCL C++"
                                                - "HCC"
                                                - "OpenMP"

     "LanguageVersion" sequence of              - The first integer is the major
                       2 integers                 version.
                                                - The second integer is the
                                                  minor version.
     "Attrs"           mapping                  Mapping of kernel attributes.
                                                See
                                                :ref:`amdgpu-amdhsa-code-object-kernel-attribute-metadata-map-v2-table`
                                                for the mapping definition.
     "Args"            sequence of              Sequence of mappings of the
                       mapping                  kernel arguments. See
                                                :ref:`amdgpu-amdhsa-code-object-kernel-argument-metadata-map-v2-table`
                                                for the definition of the mapping.
     "CodeProps"       mapping                  Mapping of properties related to
                                                the kernel code. See
                                                :ref:`amdgpu-amdhsa-code-object-kernel-code-properties-metadata-map-v2-table`
                                                for the mapping definition.
     ================= ============== ========= ================================

..

  .. table:: AMDHSA Code Object V2 Kernel Attribute Metadata Map
     :name: amdgpu-amdhsa-code-object-kernel-attribute-metadata-map-v2-table

     =================== ============== ========= ==============================
     String Key          Value Type     Required? Description
     =================== ============== ========= ==============================
     "ReqdWorkGroupSize" sequence of              If not 0, 0, 0 then all values
                         3 integers               must be >=1 and the dispatch
                                                  work-group size X, Y, Z must
                                                  correspond to the specified
                                                  values. Defaults to 0, 0, 0.

                                                  Corresponds to the OpenCL
                                                  ``reqd_work_group_size``
                                                  attribute.
     "WorkGroupSizeHint" sequence of              The dispatch work-group size
                         3 integers               X, Y, Z is likely to be the
                                                  specified values.

                                                  Corresponds to the OpenCL
                                                  ``work_group_size_hint``
                                                  attribute.
     "VecTypeHint"       string                   The name of a scalar or vector
                                                  type.

                                                  Corresponds to the OpenCL
                                                  ``vec_type_hint`` attribute.

     "RuntimeHandle"     string                   The external symbol name
                                                  associated with a kernel.
                                                  OpenCL runtime allocates a
                                                  global buffer for the symbol
                                                  and saves the kernel's address
                                                  to it, which is used for
                                                  device side enqueueing. Only
                                                  available for device side
                                                  enqueued kernels.
     =================== ============== ========= ==============================

..

  .. table:: AMDHSA Code Object V2 Kernel Argument Metadata Map
     :name: amdgpu-amdhsa-code-object-kernel-argument-metadata-map-v2-table

     ================= ============== ========= ================================
     String Key        Value Type     Required? Description
     ================= ============== ========= ================================
     "Name"            string                   Kernel argument name.
     "TypeName"        string                   Kernel argument type name.
     "Size"            integer        Required  Kernel argument size in bytes.
     "Align"           integer        Required  Kernel argument alignment in
                                                bytes. Must be a power of two.
     "ValueKind"       string         Required  Kernel argument kind that
                                                specifies how to set up the
                                                corresponding argument.
                                                Values include:

                                                "ByValue"
                                                  The argument is copied
                                                  directly into the kernarg.

                                                "GlobalBuffer"
                                                  A global address space pointer
                                                  to the buffer data is passed
                                                  in the kernarg.

                                                "DynamicSharedPointer"
                                                  A group address space pointer
                                                  to dynamically allocated LDS
                                                  is passed in the kernarg.

                                                "Sampler"
                                                  A global address space
                                                  pointer to a S# is passed in
                                                  the kernarg.

                                                "Image"
                                                  A global address space
                                                  pointer to a T# is passed in
                                                  the kernarg.

                                                "Pipe"
                                                  A global address space pointer
                                                  to an OpenCL pipe is passed in
                                                  the kernarg.

                                                "Queue"
                                                  A global address space pointer
                                                  to an OpenCL device enqueue
                                                  queue is passed in the
                                                  kernarg.

                                                "HiddenGlobalOffsetX"
                                                  The OpenCL grid dispatch
                                                  global offset for the X
                                                  dimension is passed in the
                                                  kernarg.

                                                "HiddenGlobalOffsetY"
                                                  The OpenCL grid dispatch
                                                  global offset for the Y
                                                  dimension is passed in the
                                                  kernarg.

                                                "HiddenGlobalOffsetZ"
                                                  The OpenCL grid dispatch
                                                  global offset for the Z
                                                  dimension is passed in the
                                                  kernarg.

                                                "HiddenNone"
                                                  An argument that is not used
                                                  by the kernel. Space needs to
                                                  be left for it, but it does
                                                  not need to be set up.

                                                "HiddenPrintfBuffer"
                                                  A global address space pointer
                                                  to the runtime printf buffer
                                                  is passed in kernarg. Mutually
                                                  exclusive with
                                                  "HiddenHostcallBuffer".

                                                "HiddenHostcallBuffer"
                                                  A global address space pointer
                                                  to the runtime hostcall buffer
                                                  is passed in kernarg. Mutually
                                                  exclusive with
                                                  "HiddenPrintfBuffer".

                                                "HiddenDefaultQueue"
                                                  A global address space pointer
                                                  to the OpenCL device enqueue
                                                  queue that should be used by
                                                  the kernel by default is
                                                  passed in the kernarg.

                                                "HiddenCompletionAction"
                                                  A global address space pointer
                                                  to help link enqueued kernels into
                                                  the ancestor tree for determining
                                                  when the parent kernel has finished.

                                                "HiddenMultiGridSyncArg"
                                                  A global address space pointer for
                                                  multi-grid synchronization is
                                                  passed in the kernarg.

     "ValueType"       string                   Unused and deprecated. This should no longer
                                                be emitted, but is accepted for compatibility.


     "PointeeAlign"    integer                  Alignment in bytes of pointee
                                                type for pointer type kernel
                                                argument. Must be a power
                                                of 2. Only present if
                                                "ValueKind" is
                                                "DynamicSharedPointer".
     "AddrSpaceQual"   string                   Kernel argument address space
                                                qualifier. Only present if
                                                "ValueKind" is "GlobalBuffer" or
                                                "DynamicSharedPointer". Values
                                                are:

                                                - "Private"
                                                - "Global"
                                                - "Constant"
                                                - "Local"
                                                - "Generic"
                                                - "Region"

                                                .. TODO::

                                                   Is GlobalBuffer only Global
                                                   or Constant? Is
                                                   DynamicSharedPointer always
                                                   Local? Can HCC allow Generic?
                                                   How can Private or Region
                                                   ever happen?

     "AccQual"         string                   Kernel argument access
                                                qualifier. Only present if
                                                "ValueKind" is "Image" or
                                                "Pipe". Values
                                                are:

                                                - "ReadOnly"
                                                - "WriteOnly"
                                                - "ReadWrite"

                                                .. TODO::

                                                   Does this apply to
                                                   GlobalBuffer?

     "ActualAccQual"   string                   The actual memory accesses
                                                performed by the kernel on the
                                                kernel argument. Only present if
                                                "ValueKind" is "GlobalBuffer",
                                                "Image", or "Pipe". This may be
                                                more restrictive than indicated
                                                by "AccQual" to reflect what the
                                                kernel actual does. If not
                                                present then the runtime must
                                                assume what is implied by
                                                "AccQual" and "IsConst". Values
                                                are:

                                                - "ReadOnly"
                                                - "WriteOnly"
                                                - "ReadWrite"

     "IsConst"         boolean                  Indicates if the kernel argument
                                                is const qualified. Only present
                                                if "ValueKind" is
                                                "GlobalBuffer".

     "IsRestrict"      boolean                  Indicates if the kernel argument
                                                is restrict qualified. Only
                                                present if "ValueKind" is
                                                "GlobalBuffer".

     "IsVolatile"      boolean                  Indicates if the kernel argument
                                                is volatile qualified. Only
                                                present if "ValueKind" is
                                                "GlobalBuffer".

     "IsPipe"          boolean                  Indicates if the kernel argument
                                                is pipe qualified. Only present
                                                if "ValueKind" is "Pipe".

                                                .. TODO::

                                                   Can GlobalBuffer be pipe
                                                   qualified?

     ================= ============== ========= ================================

..

  .. table:: AMDHSA Code Object V2 Kernel Code Properties Metadata Map
     :name: amdgpu-amdhsa-code-object-kernel-code-properties-metadata-map-v2-table

     ============================ ============== ========= =====================
     String Key                   Value Type     Required? Description
     ============================ ============== ========= =====================
     "KernargSegmentSize"         integer        Required  The size in bytes of
                                                           the kernarg segment
                                                           that holds the values
                                                           of the arguments to
                                                           the kernel.
     "GroupSegmentFixedSize"      integer        Required  The amount of group
                                                           segment memory
                                                           required by a
                                                           work-group in
                                                           bytes. This does not
                                                           include any
                                                           dynamically allocated
                                                           group segment memory
                                                           that may be added
                                                           when the kernel is
                                                           dispatched.
     "PrivateSegmentFixedSize"    integer        Required  The amount of fixed
                                                           private address space
                                                           memory required for a
                                                           work-item in
                                                           bytes. If the kernel
                                                           uses a dynamic call
                                                           stack then additional
                                                           space must be added
                                                           to this value for the
                                                           call stack.
     "KernargSegmentAlign"        integer        Required  The maximum byte
                                                           alignment of
                                                           arguments in the
                                                           kernarg segment. Must
                                                           be a power of 2.
     "WavefrontSize"              integer        Required  Wavefront size. Must
                                                           be a power of 2.
     "NumSGPRs"                   integer        Required  Number of scalar
                                                           registers used by a
                                                           wavefront for
                                                           GFX6-GFX11. This
                                                           includes the special
                                                           SGPRs for VCC, Flat
                                                           Scratch (GFX7-GFX10)
                                                           and XNACK (for
                                                           GFX8-GFX10). It does
                                                           not include the 16
                                                           SGPR added if a trap
                                                           handler is
                                                           enabled. It is not
                                                           rounded up to the
                                                           allocation
                                                           granularity.
     "NumVGPRs"                   integer        Required  Number of vector
                                                           registers used by
                                                           each work-item for
                                                           GFX6-GFX11
     "MaxFlatWorkGroupSize"       integer        Required  Maximum flat
                                                           work-group size
                                                           supported by the
                                                           kernel in work-items.
                                                           Must be >=1 and
                                                           consistent with
                                                           ReqdWorkGroupSize if
                                                           not 0, 0, 0.
     "NumSpilledSGPRs"            integer                  Number of stores from
                                                           a scalar register to
                                                           a register allocator
                                                           created spill
                                                           location.
     "NumSpilledVGPRs"            integer                  Number of stores from
                                                           a vector register to
                                                           a register allocator
                                                           created spill
                                                           location.
     ============================ ============== ========= =====================

.. _amdgpu-amdhsa-code-object-metadata-v3:

Code Object V3 Metadata
+++++++++++++++++++++++

.. warning::
  Code object V3 is not the default code object version emitted by this version
  of LLVM.

Code object V3 and above metadata is specified by the ``NT_AMDGPU_METADATA`` note
record (see :ref:`amdgpu-note-records-v3-onwards`).

The metadata is represented as Message Pack formatted binary data (see
[MsgPack]_). The top level is a Message Pack map that includes the
keys defined in table
:ref:`amdgpu-amdhsa-code-object-metadata-map-table-v3` and referenced
tables.

Additional information can be added to the maps. To avoid conflicts,
any key names should be prefixed by "*vendor-name*." where
``vendor-name`` can be the name of the vendor and specific vendor
tool that generates the information. The prefix is abbreviated to
simply "." when it appears within a map that has been added by the
same *vendor-name*.

  .. table:: AMDHSA Code Object V3 Metadata Map
     :name: amdgpu-amdhsa-code-object-metadata-map-table-v3

     ================= ============== ========= =======================================
     String Key        Value Type     Required? Description
     ================= ============== ========= =======================================
     "amdhsa.version"  sequence of    Required  - The first integer is the major
                       2 integers                 version. Currently 1.
                                                - The second integer is the minor
                                                  version. Currently 0.
     "amdhsa.printf"   sequence of              Each string is encoded information
                       strings                  about a printf function call. The
                                                encoded information is organized as
                                                fields separated by colon (':'):

                                                ``ID:N:S[0]:S[1]:...:S[N-1]:FormatString``

                                                where:

                                                ``ID``
                                                  A 32-bit integer as a unique id for
                                                  each printf function call

                                                ``N``
                                                  A 32-bit integer equal to the number
                                                  of arguments of printf function call
                                                  minus 1

                                                ``S[i]`` (where i = 0, 1, ... , N-1)
                                                  32-bit integers for the size in bytes
                                                  of the i-th FormatString argument of
                                                  the printf function call

                                                FormatString
                                                  The format string passed to the
                                                  printf function call.
     "amdhsa.kernels"  sequence of    Required  Sequence of the maps for each
                       map                      kernel in the code object. See
                                                :ref:`amdgpu-amdhsa-code-object-kernel-metadata-map-table-v3`
                                                for the definition of the keys included
                                                in that map.
     ================= ============== ========= =======================================

..

  .. table:: AMDHSA Code Object V3 Kernel Metadata Map
     :name: amdgpu-amdhsa-code-object-kernel-metadata-map-table-v3

     =================================== ============== ========= ================================
     String Key                          Value Type     Required? Description
     =================================== ============== ========= ================================
     ".name"                             string         Required  Source name of the kernel.
     ".symbol"                           string         Required  Name of the kernel
                                                                  descriptor ELF symbol.
     ".language"                         string                   Source language of the kernel.
                                                                  Values include:

                                                                  - "OpenCL C"
                                                                  - "OpenCL C++"
                                                                  - "HCC"
                                                                  - "HIP"
                                                                  - "OpenMP"
                                                                  - "Assembler"

     ".language_version"                 sequence of              - The first integer is the major
                                         2 integers                 version.
                                                                  - The second integer is the
                                                                    minor version.
     ".args"                             sequence of              Sequence of maps of the
                                         map                      kernel arguments. See
                                                                  :ref:`amdgpu-amdhsa-code-object-kernel-argument-metadata-map-table-v3`
                                                                  for the definition of the keys
                                                                  included in that map.
     ".reqd_workgroup_size"              sequence of              If not 0, 0, 0 then all values
                                         3 integers               must be >=1 and the dispatch
                                                                  work-group size X, Y, Z must
                                                                  correspond to the specified
                                                                  values. Defaults to 0, 0, 0.

                                                                  Corresponds to the OpenCL
                                                                  ``reqd_work_group_size``
                                                                  attribute.
     ".workgroup_size_hint"              sequence of              The dispatch work-group size
                                         3 integers               X, Y, Z is likely to be the
                                                                  specified values.

                                                                  Corresponds to the OpenCL
                                                                  ``work_group_size_hint``
                                                                  attribute.
     ".vec_type_hint"                    string                   The name of a scalar or vector
                                                                  type.

                                                                  Corresponds to the OpenCL
                                                                  ``vec_type_hint`` attribute.

     ".device_enqueue_symbol"            string                   The external symbol name
                                                                  associated with a kernel.
                                                                  OpenCL runtime allocates a
                                                                  global buffer for the symbol
                                                                  and saves the kernel's address
                                                                  to it, which is used for
                                                                  device side enqueueing. Only
                                                                  available for device side
                                                                  enqueued kernels.
     ".kernarg_segment_size"             integer        Required  The size in bytes of
                                                                  the kernarg segment
                                                                  that holds the values
                                                                  of the arguments to
                                                                  the kernel.
     ".group_segment_fixed_size"         integer        Required  The amount of group
                                                                  segment memory
                                                                  required by a
                                                                  work-group in
                                                                  bytes. This does not
                                                                  include any
                                                                  dynamically allocated
                                                                  group segment memory
                                                                  that may be added
                                                                  when the kernel is
                                                                  dispatched.
     ".private_segment_fixed_size"       integer        Required  The amount of fixed
                                                                  private address space
                                                                  memory required for a
                                                                  work-item in
                                                                  bytes. If the kernel
                                                                  uses a dynamic call
                                                                  stack then additional
                                                                  space must be added
                                                                  to this value for the
                                                                  call stack.
     ".kernarg_segment_align"            integer        Required  The maximum byte
                                                                  alignment of
                                                                  arguments in the
                                                                  kernarg segment. Must
                                                                  be a power of 2.
     ".wavefront_size"                   integer        Required  Wavefront size. Must
                                                                  be a power of 2.
     ".sgpr_count"                       integer        Required  Number of scalar
                                                                  registers required by a
                                                                  wavefront for
                                                                  GFX6-GFX9. A register
                                                                  is required if it is
                                                                  used explicitly, or
                                                                  if a higher numbered
                                                                  register is used
                                                                  explicitly. This
                                                                  includes the special
                                                                  SGPRs for VCC, Flat
                                                                  Scratch (GFX7-GFX9)
                                                                  and XNACK (for
                                                                  GFX8-GFX9). It does
                                                                  not include the 16
                                                                  SGPR added if a trap
                                                                  handler is
                                                                  enabled. It is not
                                                                  rounded up to the
                                                                  allocation
                                                                  granularity.
     ".vgpr_count"                       integer        Required  Number of vector
                                                                  registers required by
                                                                  each work-item for
                                                                  GFX6-GFX9. A register
                                                                  is required if it is
                                                                  used explicitly, or
                                                                  if a higher numbered
                                                                  register is used
                                                                  explicitly.
     ".agpr_count"                       integer        Required  Number of accumulator
                                                                  registers required by
                                                                  each work-item for
                                                                  GFX90A, GFX908.
     ".max_flat_workgroup_size"          integer        Required  Maximum flat
                                                                  work-group size
                                                                  supported by the
                                                                  kernel in work-items.
                                                                  Must be >=1 and
                                                                  consistent with
                                                                  ReqdWorkGroupSize if
                                                                  not 0, 0, 0.
     ".sgpr_spill_count"                 integer                  Number of stores from
                                                                  a scalar register to
                                                                  a register allocator
                                                                  created spill
                                                                  location.
     ".vgpr_spill_count"                 integer                  Number of stores from
                                                                  a vector register to
                                                                  a register allocator
                                                                  created spill
                                                                  location.
     ".kind"                             string                   The kind of the kernel
                                                                  with the following
                                                                  values:

                                                                  "normal"
                                                                    Regular kernels.

                                                                  "init"
                                                                    These kernels must be
                                                                    invoked after loading
                                                                    the containing code
                                                                    object and must
                                                                    complete before any
                                                                    normal and fini
                                                                    kernels in the same
                                                                    code object are
                                                                    invoked.

                                                                  "fini"
                                                                    These kernels must be
                                                                    invoked before
                                                                    unloading the
                                                                    containing code object
                                                                    and after all init and
                                                                    normal kernels in the
                                                                    same code object have
                                                                    been invoked and
                                                                    completed.

                                                                  If omitted, "normal" is
                                                                  assumed.
     =================================== ============== ========= ================================

..

  .. table:: AMDHSA Code Object V3 Kernel Argument Metadata Map
     :name: amdgpu-amdhsa-code-object-kernel-argument-metadata-map-table-v3

     ====================== ============== ========= ================================
     String Key             Value Type     Required? Description
     ====================== ============== ========= ================================
     ".name"                string                   Kernel argument name.
     ".type_name"           string                   Kernel argument type name.
     ".size"                integer        Required  Kernel argument size in bytes.
     ".offset"              integer        Required  Kernel argument offset in
                                                     bytes. The offset must be a
                                                     multiple of the alignment
                                                     required by the argument.
     ".value_kind"          string         Required  Kernel argument kind that
                                                     specifies how to set up the
                                                     corresponding argument.
                                                     Values include:

                                                     "by_value"
                                                       The argument is copied
                                                       directly into the kernarg.

                                                     "global_buffer"
                                                       A global address space pointer
                                                       to the buffer data is passed
                                                       in the kernarg.

                                                     "dynamic_shared_pointer"
                                                       A group address space pointer
                                                       to dynamically allocated LDS
                                                       is passed in the kernarg.

                                                     "sampler"
                                                       A global address space
                                                       pointer to a S# is passed in
                                                       the kernarg.

                                                     "image"
                                                       A global address space
                                                       pointer to a T# is passed in
                                                       the kernarg.

                                                     "pipe"
                                                       A global address space pointer
                                                       to an OpenCL pipe is passed in
                                                       the kernarg.

                                                     "queue"
                                                       A global address space pointer
                                                       to an OpenCL device enqueue
                                                       queue is passed in the
                                                       kernarg.

                                                     "hidden_global_offset_x"
                                                       The OpenCL grid dispatch
                                                       global offset for the X
                                                       dimension is passed in the
                                                       kernarg.

                                                     "hidden_global_offset_y"
                                                       The OpenCL grid dispatch
                                                       global offset for the Y
                                                       dimension is passed in the
                                                       kernarg.

                                                     "hidden_global_offset_z"
                                                       The OpenCL grid dispatch
                                                       global offset for the Z
                                                       dimension is passed in the
                                                       kernarg.

                                                     "hidden_none"
                                                       An argument that is not used
                                                       by the kernel. Space needs to
                                                       be left for it, but it does
                                                       not need to be set up.

                                                     "hidden_printf_buffer"
                                                       A global address space pointer
                                                       to the runtime printf buffer
                                                       is passed in kernarg. Mutually
                                                       exclusive with
                                                       "hidden_hostcall_buffer"
                                                       before Code Object V5.

                                                     "hidden_hostcall_buffer"
                                                       A global address space pointer
                                                       to the runtime hostcall buffer
                                                       is passed in kernarg. Mutually
                                                       exclusive with
                                                       "hidden_printf_buffer"
                                                       before Code Object V5.

                                                     "hidden_default_queue"
                                                       A global address space pointer
                                                       to the OpenCL device enqueue
                                                       queue that should be used by
                                                       the kernel by default is
                                                       passed in the kernarg.

                                                     "hidden_completion_action"
                                                       A global address space pointer
                                                       to help link enqueued kernels into
                                                       the ancestor tree for determining
                                                       when the parent kernel has finished.

                                                     "hidden_multigrid_sync_arg"
                                                       A global address space pointer for
                                                       multi-grid synchronization is
                                                       passed in the kernarg.

     ".value_type"          string                    Unused and deprecated. This should no longer
                                                      be emitted, but is accepted for compatibility.

     ".pointee_align"       integer                  Alignment in bytes of pointee
                                                     type for pointer type kernel
                                                     argument. Must be a power
                                                     of 2. Only present if
                                                     ".value_kind" is
                                                     "dynamic_shared_pointer".
     ".address_space"       string                   Kernel argument address space
                                                     qualifier. Only present if
                                                     ".value_kind" is "global_buffer" or
                                                     "dynamic_shared_pointer". Values
                                                     are:

                                                     - "private"
                                                     - "global"
                                                     - "constant"
                                                     - "local"
                                                     - "generic"
                                                     - "region"

                                                     .. TODO::

                                                        Is "global_buffer" only "global"
                                                        or "constant"? Is
                                                        "dynamic_shared_pointer" always
                                                        "local"? Can HCC allow "generic"?
                                                        How can "private" or "region"
                                                        ever happen?

     ".access"              string                   Kernel argument access
                                                     qualifier. Only present if
                                                     ".value_kind" is "image" or
                                                     "pipe". Values
                                                     are:

                                                     - "read_only"
                                                     - "write_only"
                                                     - "read_write"

                                                     .. TODO::

                                                        Does this apply to
                                                        "global_buffer"?

     ".actual_access"       string                   The actual memory accesses
                                                     performed by the kernel on the
                                                     kernel argument. Only present if
                                                     ".value_kind" is "global_buffer",
                                                     "image", or "pipe". This may be
                                                     more restrictive than indicated
                                                     by ".access" to reflect what the
                                                     kernel actual does. If not
                                                     present then the runtime must
                                                     assume what is implied by
                                                     ".access" and ".is_const"      . Values
                                                     are:

                                                     - "read_only"
                                                     - "write_only"
                                                     - "read_write"

     ".is_const"            boolean                  Indicates if the kernel argument
                                                     is const qualified. Only present
                                                     if ".value_kind" is
                                                     "global_buffer".

     ".is_restrict"         boolean                  Indicates if the kernel argument
                                                     is restrict qualified. Only
                                                     present if ".value_kind" is
                                                     "global_buffer".

     ".is_volatile"         boolean                  Indicates if the kernel argument
                                                     is volatile qualified. Only
                                                     present if ".value_kind" is
                                                     "global_buffer".

     ".is_pipe"             boolean                  Indicates if the kernel argument
                                                     is pipe qualified. Only present
                                                     if ".value_kind" is "pipe".

                                                     .. TODO::

                                                        Can "global_buffer" be pipe
                                                        qualified?

     ====================== ============== ========= ================================

.. _amdgpu-amdhsa-code-object-metadata-v4:

Code Object V4 Metadata
+++++++++++++++++++++++

Code object V4 metadata is the same as
:ref:`amdgpu-amdhsa-code-object-metadata-v3` with the changes and additions
defined in table :ref:`amdgpu-amdhsa-code-object-metadata-map-table-v4`.

  .. table:: AMDHSA Code Object V4 Metadata Map Changes
     :name: amdgpu-amdhsa-code-object-metadata-map-table-v4

     ================= ============== ========= =======================================
     String Key        Value Type     Required? Description
     ================= ============== ========= =======================================
     "amdhsa.version"  sequence of    Required  - The first integer is the major
                       2 integers                 version. Currently 1.
                                                - The second integer is the minor
                                                  version. Currently 1.
     "amdhsa.target"   string         Required  The target name of the code using the syntax:

                                                .. code::

                                                  <target-triple> [ "-" <target-id> ]

                                                A canonical target ID must be
                                                used. See :ref:`amdgpu-target-triples`
                                                and :ref:`amdgpu-target-id`.
     ================= ============== ========= =======================================

.. _amdgpu-amdhsa-code-object-metadata-v5:

Code Object V5 Metadata
+++++++++++++++++++++++

.. warning::
  Code object V5 is not the default code object version emitted by this version
  of LLVM.


Code object V5 metadata is the same as
:ref:`amdgpu-amdhsa-code-object-metadata-v4` with the changes defined in table
:ref:`amdgpu-amdhsa-code-object-metadata-map-table-v5`, table
:ref:`amdgpu-amdhsa-code-object-kernel-metadata-map-table-v5` and table
:ref:`amdgpu-amdhsa-code-object-kernel-argument-metadata-map-table-v5`.

  .. table:: AMDHSA Code Object V5 Metadata Map Changes
     :name: amdgpu-amdhsa-code-object-metadata-map-table-v5

     ================= ============== ========= =======================================
     String Key        Value Type     Required? Description
     ================= ============== ========= =======================================
     "amdhsa.version"  sequence of    Required  - The first integer is the major
                       2 integers                 version. Currently 1.
                                                - The second integer is the minor
                                                  version. Currently 2.
     ================= ============== ========= =======================================

..

  .. table:: AMDHSA Code Object V5 Kernel Metadata Map Additions
     :name: amdgpu-amdhsa-code-object-kernel-metadata-map-table-v5

     ============================= ============= ========== =======================================
     String Key                    Value Type     Required? Description
     ============================= ============= ========== =======================================
     ".uses_dynamic_stack"         boolean                  Indicates if the generated machine code
                                                            is using a dynamically sized stack.
     ".workgroup_processor_mode"   boolean                  (GFX10+) Controls ENABLE_WGP_MODE in
                                                            :ref:`amdgpu-amdhsa-kernel-descriptor-v3-table`.
     ============================= ============= ========== =======================================

..

  .. table:: AMDHSA Code Object V5 Kernel Attribute Metadata Map
     :name: amdgpu-amdhsa-code-object-kernel-attribute-metadata-map-v5-table

     =========================== ============== ========= ==============================
     String Key                  Value Type     Required? Description
     =========================== ============== ========= ==============================
     ".uniform_work_group_size"  integer                  Indicates if the kernel
                                                          requires that each dimension
                                                          of global size is a multiple
                                                          of corresponding dimension of
                                                          work-group size. Value of 1
                                                          implies true and value of 0
                                                          implies false. Metadata is
                                                          only emitted when value is 1.
     =========================== ============== ========= ==============================

..

..

  .. table:: AMDHSA Code Object V5 Kernel Argument Metadata Map Additions and Changes
     :name: amdgpu-amdhsa-code-object-kernel-argument-metadata-map-table-v5

     ====================== ============== ========= ================================
     String Key             Value Type     Required? Description
     ====================== ============== ========= ================================
     ".value_kind"          string         Required  Kernel argument kind that
                                                     specifies how to set up the
                                                     corresponding argument.
                                                     Values include:
                                                     the same as code object V3 metadata
                                                     (see :ref:`amdgpu-amdhsa-code-object-kernel-argument-metadata-map-table-v3`)
                                                     with the following additions:

                                                     "hidden_block_count_x"
                                                       The grid dispatch work-group count for the X dimension
                                                       is passed in the kernarg. Some languages, such as OpenCL,
                                                       support a last work-group in each dimension being partial.
                                                       This count only includes the non-partial work-group count.
                                                       This is not the same as the value in the AQL dispatch packet,
                                                       which has the grid size in work-items.

                                                     "hidden_block_count_y"
                                                       The grid dispatch work-group count for the Y dimension
                                                       is passed in the kernarg. Some languages, such as OpenCL,
                                                       support a last work-group in each dimension being partial.
                                                       This count only includes the non-partial work-group count.
                                                       This is not the same as the value in the AQL dispatch packet,
                                                       which has the grid size in work-items. If the grid dimensionality
                                                       is 1, then must be 1.

                                                     "hidden_block_count_z"
                                                       The grid dispatch work-group count for the Z dimension
                                                       is passed in the kernarg. Some languages, such as OpenCL,
                                                       support a last work-group in each dimension being partial.
                                                       This count only includes the non-partial work-group count.
                                                       This is not the same as the value in the AQL dispatch packet,
                                                       which has the grid size in work-items. If the grid dimensionality
                                                       is 1 or 2, then must be 1.

                                                     "hidden_group_size_x"
                                                       The grid dispatch work-group size for the X dimension is
                                                       passed in the kernarg. This size only applies to the
                                                       non-partial work-groups. This is the same value as the AQL
                                                       dispatch packet work-group size.

                                                     "hidden_group_size_y"
                                                       The grid dispatch work-group size for the Y dimension is
                                                       passed in the kernarg. This size only applies to the
                                                       non-partial work-groups. This is the same value as the AQL
                                                       dispatch packet work-group size. If the grid dimensionality
                                                       is 1, then must be 1.

                                                     "hidden_group_size_z"
                                                       The grid dispatch work-group size for the Z dimension is
                                                       passed in the kernarg. This size only applies to the
                                                       non-partial work-groups. This is the same value as the AQL
                                                       dispatch packet work-group size. If the grid dimensionality
                                                       is 1 or 2, then must be 1.

                                                     "hidden_remainder_x"
                                                       The grid dispatch work group size of the partial work group
                                                       of the X dimension, if it exists. Must be zero if a partial
                                                       work group does not exist in the X dimension.

                                                     "hidden_remainder_y"
                                                       The grid dispatch work group size of the partial work group
                                                       of the Y dimension, if it exists. Must be zero if a partial
                                                       work group does not exist in the Y dimension.

                                                     "hidden_remainder_z"
                                                       The grid dispatch work group size of the partial work group
                                                       of the Z dimension, if it exists. Must be zero if a partial
                                                       work group does not exist in the Z dimension.

                                                     "hidden_grid_dims"
                                                       The grid dispatch dimensionality. This is the same value
                                                       as the AQL dispatch packet dimensionality. Must be a value
                                                       between 1 and 3.

                                                     "hidden_heap_v1"
                                                       A global address space pointer to an initialized memory
                                                       buffer that conforms to the requirements of the malloc/free
                                                       device library V1 version implementation.

                                                     "hidden_private_base"
                                                       The high 32 bits of the flat addressing private aperture base.
                                                       Only used by GFX8 to allow conversion between private segment
                                                       and flat addresses. See :ref:`amdgpu-amdhsa-kernel-prolog-flat-scratch`.

                                                     "hidden_shared_base"
                                                       The high 32 bits of the flat addressing shared aperture base.
                                                       Only used by GFX8 to allow conversion between shared segment
                                                       and flat addresses. See :ref:`amdgpu-amdhsa-kernel-prolog-flat-scratch`.

                                                     "hidden_queue_ptr"
                                                       A global memory address space pointer to the ROCm runtime
                                                       ``struct amd_queue_t`` structure for the HSA queue of the
                                                       associated dispatch AQL packet. It is only required for pre-GFX9
                                                       devices for the trap handler ABI (see :ref:`amdgpu-amdhsa-trap-handler-abi`).

     ====================== ============== ========= ================================

..

Kernel Dispatch
~~~~~~~~~~~~~~~

The HSA architected queuing language (AQL) defines a user space memory interface
that can be used to control the dispatch of kernels, in an agent independent
way. An agent can have zero or more AQL queues created for it using an HSA
compatible runtime (see :ref:`amdgpu-os`), in which AQL packets (all of which
are 64 bytes) can be placed. See the *HSA Platform System Architecture
Specification* [HSA]_ for the AQL queue mechanics and packet layouts.

The packet processor of a kernel agent is responsible for detecting and
dispatching HSA kernels from the AQL queues associated with it. For AMD GPUs the
packet processor is implemented by the hardware command processor (CP),
asynchronous dispatch controller (ADC) and shader processor input controller
(SPI).

An HSA compatible runtime can be used to allocate an AQL queue object. It uses
the kernel mode driver to initialize and register the AQL queue with CP.

To dispatch a kernel the following actions are performed. This can occur in the
CPU host program, or from an HSA kernel executing on a GPU.

1. A pointer to an AQL queue for the kernel agent on which the kernel is to be
   executed is obtained.
2. A pointer to the kernel descriptor (see
   :ref:`amdgpu-amdhsa-kernel-descriptor`) of the kernel to execute is obtained.
   It must be for a kernel that is contained in a code object that was loaded
   by an HSA compatible runtime on the kernel agent with which the AQL queue is
   associated.
3. Space is allocated for the kernel arguments using the HSA compatible runtime
   allocator for a memory region with the kernarg property for the kernel agent
   that will execute the kernel. It must be at least 16-byte aligned.
4. Kernel argument values are assigned to the kernel argument memory
   allocation. The layout is defined in the *HSA Programmer's Language
   Reference* [HSA]_. For AMDGPU the kernel execution directly accesses the
   kernel argument memory in the same way constant memory is accessed. (Note
   that the HSA specification allows an implementation to copy the kernel
   argument contents to another location that is accessed by the kernel.)
5. An AQL kernel dispatch packet is created on the AQL queue. The HSA compatible
   runtime api uses 64-bit atomic operations to reserve space in the AQL queue
   for the packet. The packet must be set up, and the final write must use an
   atomic store release to set the packet kind to ensure the packet contents are
   visible to the kernel agent. AQL defines a doorbell signal mechanism to
   notify the kernel agent that the AQL queue has been updated. These rules, and
   the layout of the AQL queue and kernel dispatch packet is defined in the *HSA
   System Architecture Specification* [HSA]_.
6. A kernel dispatch packet includes information about the actual dispatch,
   such as grid and work-group size, together with information from the code
   object about the kernel, such as segment sizes. The HSA compatible runtime
   queries on the kernel symbol can be used to obtain the code object values
   which are recorded in the :ref:`amdgpu-amdhsa-code-object-metadata`.
7. CP executes micro-code and is responsible for detecting and setting up the
   GPU to execute the wavefronts of a kernel dispatch.
8. CP ensures that when the a wavefront starts executing the kernel machine
   code, the scalar general purpose registers (SGPR) and vector general purpose
   registers (VGPR) are set up as required by the machine code. The required
   setup is defined in the :ref:`amdgpu-amdhsa-kernel-descriptor`. The initial
   register state is defined in
   :ref:`amdgpu-amdhsa-initial-kernel-execution-state`.
9. The prolog of the kernel machine code (see
   :ref:`amdgpu-amdhsa-kernel-prolog`) sets up the machine state as necessary
   before continuing executing the machine code that corresponds to the kernel.
10. When the kernel dispatch has completed execution, CP signals the completion
    signal specified in the kernel dispatch packet if not 0.

.. _amdgpu-amdhsa-memory-spaces:

Memory Spaces
~~~~~~~~~~~~~

The memory space properties are:

  .. table:: AMDHSA Memory Spaces
     :name: amdgpu-amdhsa-memory-spaces-table

     ================= =========== ======== ======= ==================
     Memory Space Name HSA Segment Hardware Address NULL Value
                       Name        Name     Size
     ================= =========== ======== ======= ==================
     Private           private     scratch  32      0x00000000
     Local             group       LDS      32      0xFFFFFFFF
     Global            global      global   64      0x0000000000000000
     Constant          constant    *same as 64      0x0000000000000000
                                   global*
     Generic           flat        flat     64      0x0000000000000000
     Region            N/A         GDS      32      *not implemented
                                                    for AMDHSA*
     ================= =========== ======== ======= ==================

The global and constant memory spaces both use global virtual addresses, which
are the same virtual address space used by the CPU. However, some virtual
addresses may only be accessible to the CPU, some only accessible by the GPU,
and some by both.

Using the constant memory space indicates that the data will not change during
the execution of the kernel. This allows scalar read instructions to be
used. The vector and scalar L1 caches are invalidated of volatile data before
each kernel dispatch execution to allow constant memory to change values between
kernel dispatches.

The local memory space uses the hardware Local Data Store (LDS) which is
automatically allocated when the hardware creates work-groups of wavefronts, and
freed when all the wavefronts of a work-group have terminated. The data store
(DS) instructions can be used to access it.

The private memory space uses the hardware scratch memory support. If the kernel
uses scratch, then the hardware allocates memory that is accessed using
wavefront lane dword (4 byte) interleaving. The mapping used from private
address to physical address is:

  ``wavefront-scratch-base +
  (private-address * wavefront-size * 4) +
  (wavefront-lane-id * 4)``

There are different ways that the wavefront scratch base address is determined
by a wavefront (see :ref:`amdgpu-amdhsa-initial-kernel-execution-state`). This
memory can be accessed in an interleaved manner using buffer instruction with
the scratch buffer descriptor and per wavefront scratch offset, by the scratch
instructions, or by flat instructions. If each lane of a wavefront accesses the
same private address, the interleaving results in adjacent dwords being accessed
and hence requires fewer cache lines to be fetched. Multi-dword access is not
supported except by flat and scratch instructions in GFX9-GFX11.

The generic address space uses the hardware flat address support available in
GFX7-GFX11. This uses two fixed ranges of virtual addresses (the private and
local apertures), that are outside the range of addressible global memory, to
map from a flat address to a private or local address.

FLAT instructions can take a flat address and access global, private (scratch)
and group (LDS) memory depending on if the address is within one of the
aperture ranges. Flat access to scratch requires hardware aperture setup and
setup in the kernel prologue (see
:ref:`amdgpu-amdhsa-kernel-prolog-flat-scratch`). Flat access to LDS requires
hardware aperture setup and M0 (GFX7-GFX8) register setup (see
:ref:`amdgpu-amdhsa-kernel-prolog-m0`).

To convert between a segment address and a flat address the base address of the
apertures address can be used. For GFX7-GFX8 these are available in the
:ref:`amdgpu-amdhsa-hsa-aql-queue` the address of which can be obtained with
Queue Ptr SGPR (see :ref:`amdgpu-amdhsa-initial-kernel-execution-state`). For
GFX9-GFX11 the aperture base addresses are directly available as inline constant
registers ``SRC_SHARED_BASE/LIMIT`` and ``SRC_PRIVATE_BASE/LIMIT``. In 64 bit
address mode the aperture sizes are 2^32 bytes and the base is aligned to 2^32
which makes it easier to convert from flat to segment or segment to flat.

Image and Samplers
~~~~~~~~~~~~~~~~~~

Image and sample handles created by an HSA compatible runtime (see
:ref:`amdgpu-os`) are 64-bit addresses of a hardware 32-byte V# and 48 byte S#
object respectively. In order to support the HSA ``query_sampler`` operations
two extra dwords are used to store the HSA BRIG enumeration values for the
queries that are not trivially deducible from the S# representation.

HSA Signals
~~~~~~~~~~~

HSA signal handles created by an HSA compatible runtime (see :ref:`amdgpu-os`)
are 64-bit addresses of a structure allocated in memory accessible from both the
CPU and GPU. The structure is defined by the runtime and subject to change
between releases. For example, see [AMD-ROCm-github]_.

.. _amdgpu-amdhsa-hsa-aql-queue:

HSA AQL Queue
~~~~~~~~~~~~~

The HSA AQL queue structure is defined by an HSA compatible runtime (see
:ref:`amdgpu-os`) and subject to change between releases. For example, see
[AMD-ROCm-github]_. For some processors it contains fields needed to implement
certain language features such as the flat address aperture bases. It also
contains fields used by CP such as managing the allocation of scratch memory.

.. _amdgpu-amdhsa-kernel-descriptor:

Kernel Descriptor
~~~~~~~~~~~~~~~~~

A kernel descriptor consists of the information needed by CP to initiate the
execution of a kernel, including the entry point address of the machine code
that implements the kernel.

Code Object V3 Kernel Descriptor
++++++++++++++++++++++++++++++++

CP microcode requires the Kernel descriptor to be allocated on 64-byte
alignment.

The fields used by CP for code objects before V3 also match those specified in
:ref:`amdgpu-amdhsa-kernel-descriptor-v3-table`.

  .. table:: Code Object V3 Kernel Descriptor
     :name: amdgpu-amdhsa-kernel-descriptor-v3-table

     ======= ======= =============================== ============================
     Bits    Size    Field Name                      Description
     ======= ======= =============================== ============================
     31:0    4 bytes GROUP_SEGMENT_FIXED_SIZE        The amount of fixed local
                                                     address space memory
                                                     required for a work-group
                                                     in bytes. This does not
                                                     include any dynamically
                                                     allocated local address
                                                     space memory that may be
                                                     added when the kernel is
                                                     dispatched.
     63:32   4 bytes PRIVATE_SEGMENT_FIXED_SIZE      The amount of fixed
                                                     private address space
                                                     memory required for a
                                                     work-item in bytes.  When
                                                     this cannot be predicted,
                                                     code object v4 and older
                                                     sets this value to be
                                                     higher than the minimum
                                                     requirement.
     95:64   4 bytes KERNARG_SIZE                    The size of the kernarg
                                                     memory pointed to by the
                                                     AQL dispatch packet. The
                                                     kernarg memory is used to
                                                     pass arguments to the
                                                     kernel.

                                                     * If the kernarg pointer in
                                                       the dispatch packet is NULL
                                                       then there are no kernel
                                                       arguments.
                                                     * If the kernarg pointer in
                                                       the dispatch packet is
                                                       not NULL and this value
                                                       is 0 then the kernarg
                                                       memory size is
                                                       unspecified.
                                                     * If the kernarg pointer in
                                                       the dispatch packet is
                                                       not NULL and this value
                                                       is not 0 then the value
                                                       specifies the kernarg
                                                       memory size in bytes. It
                                                       is recommended to provide
                                                       a value as it may be used
                                                       by CP to optimize making
                                                       the kernarg memory
                                                       visible to the kernel
                                                       code.

     127:96  4 bytes                                 Reserved, must be 0.
     191:128 8 bytes KERNEL_CODE_ENTRY_BYTE_OFFSET   Byte offset (possibly
                                                     negative) from base
                                                     address of kernel
                                                     descriptor to kernel's
                                                     entry point instruction
                                                     which must be 256 byte
                                                     aligned.
     351:272 20                                      Reserved, must be 0.
             bytes
     383:352 4 bytes COMPUTE_PGM_RSRC3               GFX6-GFX9
                                                       Reserved, must be 0.
                                                     GFX90A, GFX940
                                                       Compute Shader (CS)
                                                       program settings used by
                                                       CP to set up
                                                       ``COMPUTE_PGM_RSRC3``
                                                       configuration
                                                       register. See
                                                       :ref:`amdgpu-amdhsa-compute_pgm_rsrc3-gfx90a-table`.
                                                     GFX10-GFX11
                                                       Compute Shader (CS)
                                                       program settings used by
                                                       CP to set up
                                                       ``COMPUTE_PGM_RSRC3``
                                                       configuration
                                                       register. See
                                                       :ref:`amdgpu-amdhsa-compute_pgm_rsrc3-gfx10-gfx11-table`.
     415:384 4 bytes COMPUTE_PGM_RSRC1               Compute Shader (CS)
                                                     program settings used by
                                                     CP to set up
                                                     ``COMPUTE_PGM_RSRC1``
                                                     configuration
                                                     register. See
                                                     :ref:`amdgpu-amdhsa-compute_pgm_rsrc1-gfx6-gfx11-table`.
     447:416 4 bytes COMPUTE_PGM_RSRC2               Compute Shader (CS)
                                                     program settings used by
                                                     CP to set up
                                                     ``COMPUTE_PGM_RSRC2``
                                                     configuration
                                                     register. See
                                                     :ref:`amdgpu-amdhsa-compute_pgm_rsrc2-gfx6-gfx11-table`.
     458:448 7 bits  *See separate bits below.*      Enable the setup of the
                                                     SGPR user data registers
                                                     (see
                                                     :ref:`amdgpu-amdhsa-initial-kernel-execution-state`).

                                                     The total number of SGPR
                                                     user data registers
                                                     requested must not exceed
                                                     16 and match value in
                                                     ``compute_pgm_rsrc2.user_sgpr.user_sgpr_count``.
                                                     Any requests beyond 16
                                                     will be ignored.
     >448    1 bit   ENABLE_SGPR_PRIVATE_SEGMENT     If the *Target Properties*
                     _BUFFER                         column of
                                                     :ref:`amdgpu-processor-table`
                                                     specifies *Architected flat
                                                     scratch* then not supported
                                                     and must be 0,
     >449    1 bit   ENABLE_SGPR_DISPATCH_PTR
     >450    1 bit   ENABLE_SGPR_QUEUE_PTR
     >451    1 bit   ENABLE_SGPR_KERNARG_SEGMENT_PTR
     >452    1 bit   ENABLE_SGPR_DISPATCH_ID
     >453    1 bit   ENABLE_SGPR_FLAT_SCRATCH_INIT   If the *Target Properties*
                                                     column of
                                                     :ref:`amdgpu-processor-table`
                                                     specifies *Architected flat
                                                     scratch* then not supported
                                                     and must be 0,
     >454    1 bit   ENABLE_SGPR_PRIVATE_SEGMENT
                     _SIZE
     457:455 3 bits                                  Reserved, must be 0.
     458     1 bit   ENABLE_WAVEFRONT_SIZE32         GFX6-GFX9
                                                       Reserved, must be 0.
                                                     GFX10-GFX11
                                                       - If 0 execute in
                                                         wavefront size 64 mode.
                                                       - If 1 execute in
                                                         native wavefront size
                                                         32 mode.
     459     1 bit   USES_DYNAMIC_STACK              Indicates if the generated
                                                     machine code is using a
                                                     dynamically sized stack.
                                                     This is only set in code
                                                     object v5 and later.
     463:460 1 bit                                   Reserved, must be 0.
     464     1 bit   RESERVED_464                    Deprecated, must be 0.
     467:465 3 bits                                  Reserved, must be 0.
     468     1 bit   RESERVED_468                    Deprecated, must be 0.
     469:471 3 bits                                  Reserved, must be 0.
     511:472 5 bytes                                 Reserved, must be 0.
     512     **Total size 64 bytes.**
     ======= ====================================================================

..

  .. table:: compute_pgm_rsrc1 for GFX6-GFX11
     :name: amdgpu-amdhsa-compute_pgm_rsrc1-gfx6-gfx11-table

     ======= ======= =============================== ===========================================================================
     Bits    Size    Field Name                      Description
     ======= ======= =============================== ===========================================================================
     5:0     6 bits  GRANULATED_WORKITEM_VGPR_COUNT  Number of vector register
                                                     blocks used by each work-item;
                                                     granularity is device
                                                     specific:

                                                     GFX6-GFX9
                                                       - vgprs_used 0..256
                                                       - max(0, ceil(vgprs_used / 4) - 1)
                                                     GFX90A, GFX940
                                                       - vgprs_used 0..512
                                                       - vgprs_used = align(arch_vgprs, 4)
                                                                      + acc_vgprs
                                                       - max(0, ceil(vgprs_used / 8) - 1)
                                                     GFX10-GFX11 (wavefront size 64)
                                                       - max_vgpr 1..256
                                                       - max(0, ceil(vgprs_used / 4) - 1)
                                                     GFX10-GFX11 (wavefront size 32)
                                                       - max_vgpr 1..256
                                                       - max(0, ceil(vgprs_used / 8) - 1)

                                                     Where vgprs_used is defined
                                                     as the highest VGPR number
                                                     explicitly referenced plus
                                                     one.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC1.VGPRS``.

                                                     The
                                                     :ref:`amdgpu-assembler`
                                                     calculates this
                                                     automatically for the
                                                     selected processor from
                                                     values provided to the
                                                     `.amdhsa_kernel` directive
                                                     by the
                                                     `.amdhsa_next_free_vgpr`
                                                     nested directive (see
                                                     :ref:`amdhsa-kernel-directives-table`).
     9:6     4 bits  GRANULATED_WAVEFRONT_SGPR_COUNT Number of scalar register
                                                     blocks used by a wavefront;
                                                     granularity is device
                                                     specific:

                                                     GFX6-GFX8
                                                       - sgprs_used 0..112
                                                       - max(0, ceil(sgprs_used / 8) - 1)
                                                     GFX9
                                                       - sgprs_used 0..112
                                                       - 2 * max(0, ceil(sgprs_used / 16) - 1)
                                                     GFX10-GFX11
                                                       Reserved, must be 0.
                                                       (128 SGPRs always
                                                       allocated.)

                                                     Where sgprs_used is
                                                     defined as the highest
                                                     SGPR number explicitly
                                                     referenced plus one, plus
                                                     a target specific number
                                                     of additional special
                                                     SGPRs for VCC,
                                                     FLAT_SCRATCH (GFX7+) and
                                                     XNACK_MASK (GFX8+), and
                                                     any additional
                                                     target specific
                                                     limitations. It does not
                                                     include the 16 SGPRs added
                                                     if a trap handler is
                                                     enabled.

                                                     The target specific
                                                     limitations and special
                                                     SGPR layout are defined in
                                                     the hardware
                                                     documentation, which can
                                                     be found in the
                                                     :ref:`amdgpu-processors`
                                                     table.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC1.SGPRS``.

                                                     The
                                                     :ref:`amdgpu-assembler`
                                                     calculates this
                                                     automatically for the
                                                     selected processor from
                                                     values provided to the
                                                     `.amdhsa_kernel` directive
                                                     by the
                                                     `.amdhsa_next_free_sgpr`
                                                     and `.amdhsa_reserve_*`
                                                     nested directives (see
                                                     :ref:`amdhsa-kernel-directives-table`).
     11:10   2 bits  PRIORITY                        Must be 0.

                                                     Start executing wavefront
                                                     at the specified priority.

                                                     CP is responsible for
                                                     filling in
                                                     ``COMPUTE_PGM_RSRC1.PRIORITY``.
     13:12   2 bits  FLOAT_ROUND_MODE_32             Wavefront starts execution
                                                     with specified rounding
                                                     mode for single (32
                                                     bit) floating point
                                                     precision floating point
                                                     operations.

                                                     Floating point rounding
                                                     mode values are defined in
                                                     :ref:`amdgpu-amdhsa-floating-point-rounding-mode-enumeration-values-table`.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC1.FLOAT_MODE``.
     15:14   2 bits  FLOAT_ROUND_MODE_16_64          Wavefront starts execution
                                                     with specified rounding
                                                     denorm mode for half/double (16
                                                     and 64-bit) floating point
                                                     precision floating point
                                                     operations.

                                                     Floating point rounding
                                                     mode values are defined in
                                                     :ref:`amdgpu-amdhsa-floating-point-rounding-mode-enumeration-values-table`.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC1.FLOAT_MODE``.
     17:16   2 bits  FLOAT_DENORM_MODE_32            Wavefront starts execution
                                                     with specified denorm mode
                                                     for single (32
                                                     bit)  floating point
                                                     precision floating point
                                                     operations.

                                                     Floating point denorm mode
                                                     values are defined in
                                                     :ref:`amdgpu-amdhsa-floating-point-denorm-mode-enumeration-values-table`.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC1.FLOAT_MODE``.
     19:18   2 bits  FLOAT_DENORM_MODE_16_64         Wavefront starts execution
                                                     with specified denorm mode
                                                     for half/double (16
                                                     and 64-bit) floating point
                                                     precision floating point
                                                     operations.

                                                     Floating point denorm mode
                                                     values are defined in
                                                     :ref:`amdgpu-amdhsa-floating-point-denorm-mode-enumeration-values-table`.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC1.FLOAT_MODE``.
     20      1 bit   PRIV                            Must be 0.

                                                     Start executing wavefront
                                                     in privilege trap handler
                                                     mode.

                                                     CP is responsible for
                                                     filling in
                                                     ``COMPUTE_PGM_RSRC1.PRIV``.
     21      1 bit   ENABLE_DX10_CLAMP               Wavefront starts execution
                                                     with DX10 clamp mode
                                                     enabled. Used by the vector
                                                     ALU to force DX10 style
                                                     treatment of NaN's (when
                                                     set, clamp NaN to zero,
                                                     otherwise pass NaN
                                                     through).

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC1.DX10_CLAMP``.
     22      1 bit   DEBUG_MODE                      Must be 0.

                                                     Start executing wavefront
                                                     in single step mode.

                                                     CP is responsible for
                                                     filling in
                                                     ``COMPUTE_PGM_RSRC1.DEBUG_MODE``.
     23      1 bit   ENABLE_IEEE_MODE                Wavefront starts execution
                                                     with IEEE mode
                                                     enabled. Floating point
                                                     opcodes that support
                                                     exception flag gathering
                                                     will quiet and propagate
                                                     signaling-NaN inputs per
                                                     IEEE 754-2008. Min_dx10 and
                                                     max_dx10 become IEEE
                                                     754-2008 compliant due to
                                                     signaling-NaN propagation
                                                     and quieting.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC1.IEEE_MODE``.
     24      1 bit   BULKY                           Must be 0.

                                                     Only one work-group allowed
                                                     to execute on a compute
                                                     unit.

                                                     CP is responsible for
                                                     filling in
                                                     ``COMPUTE_PGM_RSRC1.BULKY``.
     25      1 bit   CDBG_USER                       Must be 0.

                                                     Flag that can be used to
                                                     control debugging code.

                                                     CP is responsible for
                                                     filling in
                                                     ``COMPUTE_PGM_RSRC1.CDBG_USER``.
     26      1 bit   FP16_OVFL                       GFX6-GFX8
                                                       Reserved, must be 0.
                                                     GFX9-GFX11
                                                       Wavefront starts execution
                                                       with specified fp16 overflow
                                                       mode.

                                                       - If 0, fp16 overflow generates
                                                         +/-INF values.
                                                       - If 1, fp16 overflow that is the
                                                         result of an +/-INF input value
                                                         or divide by 0 produces a +/-INF,
                                                         otherwise clamps computed
                                                         overflow to +/-MAX_FP16 as
                                                         appropriate.

                                                       Used by CP to set up
                                                       ``COMPUTE_PGM_RSRC1.FP16_OVFL``.
     28:27   2 bits                                  Reserved, must be 0.
     29      1 bit    WGP_MODE                       GFX6-GFX9
                                                       Reserved, must be 0.
                                                     GFX10-GFX11
                                                       - If 0 execute work-groups in
                                                         CU wavefront execution mode.
                                                       - If 1 execute work-groups on
                                                         in WGP wavefront execution mode.

                                                       See :ref:`amdgpu-amdhsa-memory-model`.

                                                       Used by CP to set up
                                                       ``COMPUTE_PGM_RSRC1.WGP_MODE``.
     30      1 bit    MEM_ORDERED                    GFX6-GFX9
                                                       Reserved, must be 0.
                                                     GFX10-GFX11
                                                       Controls the behavior of the
                                                       s_waitcnt's vmcnt and vscnt
                                                       counters.

                                                       - If 0 vmcnt reports completion
                                                         of load and atomic with return
                                                         out of order with sample
                                                         instructions, and the vscnt
                                                         reports the completion of
                                                         store and atomic without
                                                         return in order.
                                                       - If 1 vmcnt reports completion
                                                         of load, atomic with return
                                                         and sample instructions in
                                                         order, and the vscnt reports
                                                         the completion of store and
                                                         atomic without return in order.

                                                       Used by CP to set up
                                                       ``COMPUTE_PGM_RSRC1.MEM_ORDERED``.
     31      1 bit    FWD_PROGRESS                   GFX6-GFX9
                                                       Reserved, must be 0.
                                                     GFX10-GFX11
                                                       - If 0 execute SIMD wavefronts
                                                         using oldest first policy.
                                                       - If 1 execute SIMD wavefronts to
                                                         ensure wavefronts will make some
                                                         forward progress.

                                                       Used by CP to set up
                                                       ``COMPUTE_PGM_RSRC1.FWD_PROGRESS``.
     32      **Total size 4 bytes**
     ======= ===================================================================================================================

..

  .. table:: compute_pgm_rsrc2 for GFX6-GFX11
     :name: amdgpu-amdhsa-compute_pgm_rsrc2-gfx6-gfx11-table

     ======= ======= =============================== ===========================================================================
     Bits    Size    Field Name                      Description
     ======= ======= =============================== ===========================================================================
     0       1 bit   ENABLE_PRIVATE_SEGMENT          * Enable the setup of the
                                                       private segment.
                                                     * If the *Target Properties*
                                                       column of
                                                       :ref:`amdgpu-processor-table`
                                                       does not specify
                                                       *Architected flat
                                                       scratch* then enable the
                                                       setup of the SGPR
                                                       wavefront scratch offset
                                                       system register (see
                                                       :ref:`amdgpu-amdhsa-initial-kernel-execution-state`).
                                                     * If the *Target Properties*
                                                       column of
                                                       :ref:`amdgpu-processor-table`
                                                       specifies *Architected
                                                       flat scratch* then enable
                                                       the setup of the
                                                       FLAT_SCRATCH register
                                                       pair (see
                                                       :ref:`amdgpu-amdhsa-initial-kernel-execution-state`).

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC2.SCRATCH_EN``.
     5:1     5 bits  USER_SGPR_COUNT                 The total number of SGPR
                                                     user data
                                                     registers requested. This
                                                     number must be greater than
                                                     or equal to the number of user
                                                     data registers enabled.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC2.USER_SGPR``.
     6       1 bit   ENABLE_TRAP_HANDLER             Must be 0.

                                                     This bit represents
                                                     ``COMPUTE_PGM_RSRC2.TRAP_PRESENT``,
                                                     which is set by the CP if
                                                     the runtime has installed a
                                                     trap handler.
     7       1 bit   ENABLE_SGPR_WORKGROUP_ID_X      Enable the setup of the
                                                     system SGPR register for
                                                     the work-group id in the X
                                                     dimension (see
                                                     :ref:`amdgpu-amdhsa-initial-kernel-execution-state`).

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC2.TGID_X_EN``.
     8       1 bit   ENABLE_SGPR_WORKGROUP_ID_Y      Enable the setup of the
                                                     system SGPR register for
                                                     the work-group id in the Y
                                                     dimension (see
                                                     :ref:`amdgpu-amdhsa-initial-kernel-execution-state`).

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC2.TGID_Y_EN``.
     9       1 bit   ENABLE_SGPR_WORKGROUP_ID_Z      Enable the setup of the
                                                     system SGPR register for
                                                     the work-group id in the Z
                                                     dimension (see
                                                     :ref:`amdgpu-amdhsa-initial-kernel-execution-state`).

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC2.TGID_Z_EN``.
     10      1 bit   ENABLE_SGPR_WORKGROUP_INFO      Enable the setup of the
                                                     system SGPR register for
                                                     work-group information (see
                                                     :ref:`amdgpu-amdhsa-initial-kernel-execution-state`).

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC2.TGID_SIZE_EN``.
     12:11   2 bits  ENABLE_VGPR_WORKITEM_ID         Enable the setup of the
                                                     VGPR system registers used
                                                     for the work-item ID.
                                                     :ref:`amdgpu-amdhsa-system-vgpr-work-item-id-enumeration-values-table`
                                                     defines the values.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC2.TIDIG_CMP_CNT``.
     13      1 bit   ENABLE_EXCEPTION_ADDRESS_WATCH  Must be 0.

                                                     Wavefront starts execution
                                                     with address watch
                                                     exceptions enabled which
                                                     are generated when L1 has
                                                     witnessed a thread access
                                                     an *address of
                                                     interest*.

                                                     CP is responsible for
                                                     filling in the address
                                                     watch bit in
                                                     ``COMPUTE_PGM_RSRC2.EXCP_EN_MSB``
                                                     according to what the
                                                     runtime requests.
     14      1 bit   ENABLE_EXCEPTION_MEMORY         Must be 0.

                                                     Wavefront starts execution
                                                     with memory violation
                                                     exceptions exceptions
                                                     enabled which are generated
                                                     when a memory violation has
                                                     occurred for this wavefront from
                                                     L1 or LDS
                                                     (write-to-read-only-memory,
                                                     mis-aligned atomic, LDS
                                                     address out of range,
                                                     illegal address, etc.).

                                                     CP sets the memory
                                                     violation bit in
                                                     ``COMPUTE_PGM_RSRC2.EXCP_EN_MSB``
                                                     according to what the
                                                     runtime requests.
     23:15   9 bits  GRANULATED_LDS_SIZE             Must be 0.

                                                     CP uses the rounded value
                                                     from the dispatch packet,
                                                     not this value, as the
                                                     dispatch may contain
                                                     dynamically allocated group
                                                     segment memory. CP writes
                                                     directly to
                                                     ``COMPUTE_PGM_RSRC2.LDS_SIZE``.

                                                     Amount of group segment
                                                     (LDS) to allocate for each
                                                     work-group. Granularity is
                                                     device specific:

                                                     GFX6
                                                       roundup(lds-size / (64 * 4))
                                                     GFX7-GFX11
                                                       roundup(lds-size / (128 * 4))

     24      1 bit   ENABLE_EXCEPTION_IEEE_754_FP    Wavefront starts execution
                     _INVALID_OPERATION              with specified exceptions
                                                     enabled.

                                                     Used by CP to set up
                                                     ``COMPUTE_PGM_RSRC2.EXCP_EN``
                                                     (set from bits 0..6).

                                                     IEEE 754 FP Invalid
                                                     Operation
     25      1 bit   ENABLE_EXCEPTION_FP_DENORMAL    FP Denormal one or more
                     _SOURCE                         input operands is a
                                                     denormal number
     26      1 bit   ENABLE_EXCEPTION_IEEE_754_FP    IEEE 754 FP Division by
                     _DIVISION_BY_ZERO               Zero
     27      1 bit   ENABLE_EXCEPTION_IEEE_754_FP    IEEE 754 FP FP Overflow
                     _OVERFLOW
     28      1 bit   ENABLE_EXCEPTION_IEEE_754_FP    IEEE 754 FP Underflow
                     _UNDERFLOW
     29      1 bit   ENABLE_EXCEPTION_IEEE_754_FP    IEEE 754 FP Inexact
                     _INEXACT
     30      1 bit   ENABLE_EXCEPTION_INT_DIVIDE_BY  Integer Division by Zero
                     _ZERO                           (rcp_iflag_f32 instruction
                                                     only)
     31      1 bit                                   Reserved, must be 0.
     32      **Total size 4 bytes.**
     ======= ===================================================================================================================

..

  .. table:: compute_pgm_rsrc3 for GFX90A, GFX940
     :name: amdgpu-amdhsa-compute_pgm_rsrc3-gfx90a-table

     ======= ======= =============================== ===========================================================================
     Bits    Size    Field Name                      Description
     ======= ======= =============================== ===========================================================================
     5:0     6 bits  ACCUM_OFFSET                    Offset of a first AccVGPR in the unified register file. Granularity 4.
                                                     Value 0-63. 0 - accum-offset = 4, 1 - accum-offset = 8, ...,
                                                     63 - accum-offset = 256.
     6:15    10                                      Reserved, must be 0.
             bits
     16      1 bit   TG_SPLIT                        - If 0 the waves of a work-group are
                                                       launched in the same CU.
                                                     - If 1 the waves of a work-group can be
                                                       launched in different CUs. The waves
                                                       cannot use S_BARRIER or LDS.
     17:31   15                                      Reserved, must be 0.
             bits
     32      **Total size 4 bytes.**
     ======= ===================================================================================================================

..

  .. table:: compute_pgm_rsrc3 for GFX10-GFX11
     :name: amdgpu-amdhsa-compute_pgm_rsrc3-gfx10-gfx11-table

     ======= ======= =============================== ===========================================================================
     Bits    Size    Field Name                      Description
     ======= ======= =============================== ===========================================================================
     3:0     4 bits  SHARED_VGPR_COUNT               Number of shared VGPR blocks when executing in subvector mode. For
                                                     wavefront size 64 the value is 0-15, representing 0-120 VGPRs (granularity
                                                     of 8), such that (compute_pgm_rsrc1.vgprs +1)*4 + shared_vgpr_count*8 does
                                                     not exceed 256. For wavefront size 32 shared_vgpr_count must be 0.
     9:4     6 bits  INST_PREF_SIZE                  GFX10
                                                       Reserved, must be 0.
                                                     GFX11
                                                       Number of instruction bytes to prefetch, starting at the kernel's entry
                                                       point instruction, before wavefront starts execution. The value is 0..63
                                                       with a granularity of 128 bytes.
     10      1 bit   TRAP_ON_START                   GFX10
                                                       Reserved, must be 0.
                                                     GFX11
                                                       Must be 0.

                                                       If 1, wavefront starts execution by trapping into the trap handler.

                                                       CP is responsible for filling in the trap on start bit in
                                                       ``COMPUTE_PGM_RSRC3.TRAP_ON_START`` according to what the runtime
                                                       requests.
     11      1 bit   TRAP_ON_END                     GFX10
                                                       Reserved, must be 0.
                                                     GFX11
                                                       Must be 0.

                                                       If 1, wavefront execution terminates by trapping into the trap handler.

                                                       CP is responsible for filling in the trap on end bit in
                                                       ``COMPUTE_PGM_RSRC3.TRAP_ON_END`` according to what the runtime requests.
     30:12   19 bits                                 Reserved, must be 0.
     31      1 bit   IMAGE_OP                        GFX10
                                                       Reserved, must be 0.
                                                     GFX11
                                                       If 1, the kernel execution contains image instructions. If executed as
                                                       part of a graphics pipeline, image read instructions will stall waiting
                                                       for any necessary ``WAIT_SYNC`` fence to be performed in order to
                                                       indicate that earlier pipeline stages have completed writing to the
                                                       image.

                                                       Not used for compute kernels that are not part of a graphics pipeline and
                                                       must be 0.
     32      **Total size 4 bytes.**
     ======= ===================================================================================================================

..

  .. table:: Floating Point Rounding Mode Enumeration Values
     :name: amdgpu-amdhsa-floating-point-rounding-mode-enumeration-values-table

     ====================================== ===== ==============================
     Enumeration Name                       Value Description
     ====================================== ===== ==============================
     FLOAT_ROUND_MODE_NEAR_EVEN             0     Round Ties To Even
     FLOAT_ROUND_MODE_PLUS_INFINITY         1     Round Toward +infinity
     FLOAT_ROUND_MODE_MINUS_INFINITY        2     Round Toward -infinity
     FLOAT_ROUND_MODE_ZERO                  3     Round Toward 0
     ====================================== ===== ==============================

..

  .. table:: Floating Point Denorm Mode Enumeration Values
     :name: amdgpu-amdhsa-floating-point-denorm-mode-enumeration-values-table

     ====================================== ===== ====================================
     Enumeration Name                       Value Description
     ====================================== ===== ====================================
     FLOAT_DENORM_MODE_FLUSH_SRC_DST        0     Flush Source and Destination Denorms
     FLOAT_DENORM_MODE_FLUSH_DST            1     Flush Output Denorms
     FLOAT_DENORM_MODE_FLUSH_SRC            2     Flush Source Denorms
     FLOAT_DENORM_MODE_FLUSH_NONE           3     No Flush
     ====================================== ===== ====================================

  Denormal flushing is sign respecting. i.e. the behavior expected by
  ``"denormal-fp-math"="preserve-sign"``. The behavior is undefined with
  ``"denormal-fp-math"="positive-zero"``

..

  .. table:: System VGPR Work-Item ID Enumeration Values
     :name: amdgpu-amdhsa-system-vgpr-work-item-id-enumeration-values-table

     ======================================== ===== ============================
     Enumeration Name                         Value Description
     ======================================== ===== ============================
     SYSTEM_VGPR_WORKITEM_ID_X                0     Set work-item X dimension
                                                    ID.
     SYSTEM_VGPR_WORKITEM_ID_X_Y              1     Set work-item X and Y
                                                    dimensions ID.
     SYSTEM_VGPR_WORKITEM_ID_X_Y_Z            2     Set work-item X, Y and Z
                                                    dimensions ID.
     SYSTEM_VGPR_WORKITEM_ID_UNDEFINED        3     Undefined.
     ======================================== ===== ============================

.. _amdgpu-amdhsa-initial-kernel-execution-state:

Initial Kernel Execution State
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section defines the register state that will be set up by the packet
processor prior to the start of execution of every wavefront. This is limited by
the constraints of the hardware controllers of CP/ADC/SPI.

The order of the SGPR registers is defined, but the compiler can specify which
ones are actually setup in the kernel descriptor using the ``enable_sgpr_*`` bit
fields (see :ref:`amdgpu-amdhsa-kernel-descriptor`). The register numbers used
for enabled registers are dense starting at SGPR0: the first enabled register is
SGPR0, the next enabled register is SGPR1 etc.; disabled registers do not have
an SGPR number.

The initial SGPRs comprise up to 16 User SRGPs that are set by CP and apply to
all wavefronts of the grid. It is possible to specify more than 16 User SGPRs
using the ``enable_sgpr_*`` bit fields, in which case only the first 16 are
actually initialized. These are then immediately followed by the System SGPRs
that are set up by ADC/SPI and can have different values for each wavefront of
the grid dispatch.

SGPR register initial state is defined in
:ref:`amdgpu-amdhsa-sgpr-register-set-up-order-table`.

  .. table:: SGPR Register Set Up Order
     :name: amdgpu-amdhsa-sgpr-register-set-up-order-table

     ========== ========================== ====== ==============================
     SGPR Order Name                       Number Description
                (kernel descriptor enable  of
                field)                     SGPRs
     ========== ========================== ====== ==============================
     First      Private Segment Buffer     4      See
                (enable_sgpr_private              :ref:`amdgpu-amdhsa-kernel-prolog-private-segment-buffer`.
                _segment_buffer)
     then       Dispatch Ptr               2      64-bit address of AQL dispatch
                (enable_sgpr_dispatch_ptr)        packet for kernel dispatch
                                                  actually executing.
     then       Queue Ptr                  2      64-bit address of amd_queue_t
                (enable_sgpr_queue_ptr)           object for AQL queue on which
                                                  the dispatch packet was
                                                  queued.
     then       Kernarg Segment Ptr        2      64-bit address of Kernarg
                (enable_sgpr_kernarg              segment. This is directly
                _segment_ptr)                     copied from the
                                                  kernarg_address in the kernel
                                                  dispatch packet.

                                                  Having CP load it once avoids
                                                  loading it at the beginning of
                                                  every wavefront.
     then       Dispatch Id                2      64-bit Dispatch ID of the
                (enable_sgpr_dispatch_id)         dispatch packet being
                                                  executed.
     then       Flat Scratch Init          2      See
                (enable_sgpr_flat_scratch         :ref:`amdgpu-amdhsa-kernel-prolog-flat-scratch`.
                _init)
     then       Private Segment Size       1      The 32-bit byte size of a
                (enable_sgpr_private              single work-item's memory
                _segment_size)                    allocation. This is the
                                                  value from the kernel
                                                  dispatch packet Private
                                                  Segment Byte Size rounded up
                                                  by CP to a multiple of
                                                  DWORD.

                                                  Having CP load it once avoids
                                                  loading it at the beginning of
                                                  every wavefront.

                                                  This is not used for
                                                  GFX7-GFX8 since it is the same
                                                  value as the second SGPR of
                                                  Flat Scratch Init. However, it
                                                  may be needed for GFX9-GFX11 which
                                                  changes the meaning of the
                                                  Flat Scratch Init value.
     then       Work-Group Id X            1      32-bit work-group id in X
                (enable_sgpr_workgroup_id         dimension of grid for
                _X)                               wavefront.
     then       Work-Group Id Y            1      32-bit work-group id in Y
                (enable_sgpr_workgroup_id         dimension of grid for
                _Y)                               wavefront.
     then       Work-Group Id Z            1      32-bit work-group id in Z
                (enable_sgpr_workgroup_id         dimension of grid for
                _Z)                               wavefront.
     then       Work-Group Info            1      {first_wavefront, 14'b0000,
                (enable_sgpr_workgroup            ordered_append_term[10:0],
                _info)                            threadgroup_size_in_wavefronts[5:0]}
     then       Scratch Wavefront Offset   1      See
                (enable_sgpr_private              :ref:`amdgpu-amdhsa-kernel-prolog-flat-scratch`.
                _segment_wavefront_offset)        and
                                                  :ref:`amdgpu-amdhsa-kernel-prolog-private-segment-buffer`.
     ========== ========================== ====== ==============================

The order of the VGPR registers is defined, but the compiler can specify which
ones are actually setup in the kernel descriptor using the ``enable_vgpr*`` bit
fields (see :ref:`amdgpu-amdhsa-kernel-descriptor`). The register numbers used
for enabled registers are dense starting at VGPR0: the first enabled register is
VGPR0, the next enabled register is VGPR1 etc.; disabled registers do not have a
VGPR number.

There are different methods used for the VGPR initial state:

* Unless the *Target Properties* column of :ref:`amdgpu-processor-table`
  specifies otherwise, a separate VGPR register is used per work-item ID. The
  VGPR register initial state for this method is defined in
  :ref:`amdgpu-amdhsa-vgpr-register-set-up-order-for-unpacked-work-item-id-method-table`.
* If *Target Properties* column of :ref:`amdgpu-processor-table`
  specifies *Packed work-item IDs*, the initial value of VGPR0 register is used
  for all work-item IDs. The register layout for this method is defined in
  :ref:`amdgpu-amdhsa-register-layout-for-packed-work-item-id-method-table`.

  .. table:: VGPR Register Set Up Order for Unpacked Work-Item ID Method
     :name: amdgpu-amdhsa-vgpr-register-set-up-order-for-unpacked-work-item-id-method-table

     ========== ========================== ====== ==============================
     VGPR Order Name                       Number Description
                (kernel descriptor enable  of
                field)                     VGPRs
     ========== ========================== ====== ==============================
     First      Work-Item Id X             1      32-bit work-item id in X
                (Always initialized)              dimension of work-group for
                                                  wavefront lane.
     then       Work-Item Id Y             1      32-bit work-item id in Y
                (enable_vgpr_workitem_id          dimension of work-group for
                > 0)                              wavefront lane.
     then       Work-Item Id Z             1      32-bit work-item id in Z
                (enable_vgpr_workitem_id          dimension of work-group for
                > 1)                              wavefront lane.
     ========== ========================== ====== ==============================

..

  .. table:: Register Layout for Packed Work-Item ID Method
     :name: amdgpu-amdhsa-register-layout-for-packed-work-item-id-method-table

     ======= ======= ================ =========================================
     Bits    Size    Field Name       Description
     ======= ======= ================ =========================================
     0:9     10 bits Work-Item Id X   Work-item id in X
                                      dimension of work-group for
                                      wavefront lane.

                                      Always initialized.

     10:19   10 bits Work-Item Id Y   Work-item id in Y
                                      dimension of work-group for
                                      wavefront lane.

                                      Initialized if enable_vgpr_workitem_id >
                                      0, otherwise set to 0.
     20:29   10 bits Work-Item Id Z   Work-item id in Z
                                      dimension of work-group for
                                      wavefront lane.

                                      Initialized if enable_vgpr_workitem_id >
                                      1, otherwise set to 0.
     30:31   2 bits                   Reserved, set to 0.
     ======= ======= ================ =========================================

The setting of registers is done by GPU CP/ADC/SPI hardware as follows:

1. SGPRs before the Work-Group Ids are set by CP using the 16 User Data
   registers.
2. Work-group Id registers X, Y, Z are set by ADC which supports any
   combination including none.
3. Scratch Wavefront Offset is set by SPI in a per wavefront basis which is why
   its value cannot be included with the flat scratch init value which is per
   queue (see :ref:`amdgpu-amdhsa-kernel-prolog-flat-scratch`).
4. The VGPRs are set by SPI which only supports specifying either (X), (X, Y)
   or (X, Y, Z).
5. Flat Scratch register pair initialization is described in
   :ref:`amdgpu-amdhsa-kernel-prolog-flat-scratch`.

The global segment can be accessed either using buffer instructions (GFX6 which
has V# 64-bit address support), flat instructions (GFX7-GFX11), or global
instructions (GFX9-GFX11).

If buffer operations are used, then the compiler can generate a V# with the
following properties:

* base address of 0
* no swizzle
* ATC: 1 if IOMMU present (such as APU)
* ptr64: 1
* MTYPE set to support memory coherence that matches the runtime (such as CC for
  APU and NC for dGPU).

.. _amdgpu-amdhsa-kernel-prolog:

Kernel Prolog
~~~~~~~~~~~~~

The compiler performs initialization in the kernel prologue depending on the
target and information about things like stack usage in the kernel and called
functions. Some of this initialization requires the compiler to request certain
User and System SGPRs be present in the
:ref:`amdgpu-amdhsa-initial-kernel-execution-state` via the
:ref:`amdgpu-amdhsa-kernel-descriptor`.

.. _amdgpu-amdhsa-kernel-prolog-cfi:

CFI
+++

1.  The CFI return address is undefined.

2.  The CFI CFA is defined using an expression which evaluates to a location
    description that comprises one memory location description for the
    ``DW_ASPACE_AMDGPU_private_lane`` address space address ``0``.

.. _amdgpu-amdhsa-kernel-prolog-m0:

M0
++

GFX6-GFX8
  The M0 register must be initialized with a value at least the total LDS size
  if the kernel may access LDS via DS or flat operations. Total LDS size is
  available in dispatch packet. For M0, it is also possible to use maximum
  possible value of LDS for given target (0x7FFF for GFX6 and 0xFFFF for
  GFX7-GFX8).
GFX9-GFX11
  The M0 register is not used for range checking LDS accesses and so does not
  need to be initialized in the prolog.

.. _amdgpu-amdhsa-kernel-prolog-stack-pointer:

Stack Pointer
+++++++++++++

If the kernel has function calls it must set up the ABI stack pointer described
in :ref:`amdgpu-amdhsa-function-call-convention-non-kernel-functions` by setting
SGPR32 to the unswizzled scratch offset of the address past the last local
allocation.

.. _amdgpu-amdhsa-kernel-prolog-frame-pointer:

Frame Pointer
+++++++++++++

If the kernel needs a frame pointer for the reasons defined in
``SIFrameLowering`` then SGPR33 is used and is always set to ``0`` in the
kernel prolog. If a frame pointer is not required then all uses of the frame
pointer are replaced with immediate ``0`` offsets.

.. _amdgpu-amdhsa-kernel-prolog-flat-scratch:

Flat Scratch
++++++++++++

There are different methods used for initializing flat scratch:

* If the *Target Properties* column of :ref:`amdgpu-processor-table`
  specifies *Does not support generic address space*:

  Flat scratch is not supported and there is no flat scratch register pair.

* If the *Target Properties* column of :ref:`amdgpu-processor-table`
  specifies *Offset flat scratch*:

  If the kernel or any function it calls may use flat operations to access
  scratch memory, the prolog code must set up the FLAT_SCRATCH register pair
  (FLAT_SCRATCH_LO/FLAT_SCRATCH_HI). Initialization uses Flat Scratch Init and
  Scratch Wavefront Offset SGPR registers (see
  :ref:`amdgpu-amdhsa-initial-kernel-execution-state`):

  1. The low word of Flat Scratch Init is the 32-bit byte offset from
     ``SH_HIDDEN_PRIVATE_BASE_VIMID`` to the base of scratch backing memory
     being managed by SPI for the queue executing the kernel dispatch. This is
     the same value used in the Scratch Segment Buffer V# base address.

     CP obtains this from the runtime. (The Scratch Segment Buffer base address
     is ``SH_HIDDEN_PRIVATE_BASE_VIMID`` plus this offset.)

     The prolog must add the value of Scratch Wavefront Offset to get the
     wavefront's byte scratch backing memory offset from
     ``SH_HIDDEN_PRIVATE_BASE_VIMID``.

     The Scratch Wavefront Offset must also be used as an offset with Private
     segment address when using the Scratch Segment Buffer.

     Since FLAT_SCRATCH_LO is in units of 256 bytes, the offset must be right
     shifted by 8 before moving into FLAT_SCRATCH_HI.

     FLAT_SCRATCH_HI corresponds to SGPRn-4 on GFX7, and SGPRn-6 on GFX8 (where
     SGPRn is the highest numbered SGPR allocated to the wavefront).
     FLAT_SCRATCH_HI is multiplied by 256 (as it is in units of 256 bytes) and
     added to ``SH_HIDDEN_PRIVATE_BASE_VIMID`` to calculate the per wavefront
     FLAT SCRATCH BASE in flat memory instructions that access the scratch
     aperture.
  2. The second word of Flat Scratch Init is 32-bit byte size of a single
     work-items scratch memory usage.

     CP obtains this from the runtime, and it is always a multiple of DWORD. CP
     checks that the value in the kernel dispatch packet Private Segment Byte
     Size is not larger and requests the runtime to increase the queue's scratch
     size if necessary.

     CP directly loads from the kernel dispatch packet Private Segment Byte Size
     field and rounds up to a multiple of DWORD. Having CP load it once avoids
     loading it at the beginning of every wavefront.

     The kernel prolog code must move it to FLAT_SCRATCH_LO which is SGPRn-3 on
     GFX7 and SGPRn-5 on GFX8. FLAT_SCRATCH_LO is used as the FLAT SCRATCH SIZE
     in flat memory instructions.

* If the *Target Properties* column of :ref:`amdgpu-processor-table`
  specifies *Absolute flat scratch*:

  If the kernel or any function it calls may use flat operations to access
  scratch memory, the prolog code must set up the FLAT_SCRATCH register pair
  (FLAT_SCRATCH_LO/FLAT_SCRATCH_HI which are in SGPRn-4/SGPRn-3). Initialization
  uses Flat Scratch Init and Scratch Wavefront Offset SGPR registers (see
  :ref:`amdgpu-amdhsa-initial-kernel-execution-state`):

  The Flat Scratch Init is the 64-bit address of the base of scratch backing
  memory being managed by SPI for the queue executing the kernel dispatch.

  CP obtains this from the runtime.

  The kernel prolog must add the value of the wave's Scratch Wavefront Offset
  and move the result as a 64-bit value to the FLAT_SCRATCH SGPR register pair
  which is SGPRn-6 and SGPRn-5. It is used as the FLAT SCRATCH BASE in flat
  memory instructions.

  The Scratch Wavefront Offset must also be used as an offset with Private
  segment address when using the Scratch Segment Buffer (see
  :ref:`amdgpu-amdhsa-kernel-prolog-private-segment-buffer`).

* If the *Target Properties* column of :ref:`amdgpu-processor-table`
  specifies *Architected flat scratch*:

  If ENABLE_PRIVATE_SEGMENT is enabled in
  :ref:`amdgpu-amdhsa-compute_pgm_rsrc2-gfx6-gfx11-table` then the FLAT_SCRATCH
  register pair will be initialized to the 64-bit address of the base of scratch
  backing memory being managed by SPI for the queue executing the kernel
  dispatch plus the value of the wave's Scratch Wavefront Offset for use as the
  flat scratch base in flat memory instructions.

.. _amdgpu-amdhsa-kernel-prolog-private-segment-buffer:

Private Segment Buffer
++++++++++++++++++++++

If the *Target Properties* column of :ref:`amdgpu-processor-table` specifies
*Architected flat scratch* then a Private Segment Buffer is not supported.
Instead the flat SCRATCH instructions are used.

Otherwise, Private Segment Buffer SGPR register is used to initialize 4 SGPRs
that are used as a V# to access scratch. CP uses the value provided by the
runtime. It is used, together with Scratch Wavefront Offset as an offset, to
access the private memory space using a segment address. See
:ref:`amdgpu-amdhsa-initial-kernel-execution-state`.

The scratch V# is a four-aligned SGPR and always selected for the kernel as
follows:

  - If it is known during instruction selection that there is stack usage,
    SGPR0-3 is reserved for use as the scratch V#.  Stack usage is assumed if
    optimizations are disabled (``-O0``), if stack objects already exist (for
    locals, etc.), or if there are any function calls.

  - Otherwise, four high numbered SGPRs beginning at a four-aligned SGPR index
    are reserved for the tentative scratch V#. These will be used if it is
    determined that spilling is needed.

    - If no use is made of the tentative scratch V#, then it is unreserved,
      and the register count is determined ignoring it.
    - If use is made of the tentative scratch V#, then its register numbers
      are shifted to the first four-aligned SGPR index after the highest one
      allocated by the register allocator, and all uses are updated. The
      register count includes them in the shifted location.
    - In either case, if the processor has the SGPR allocation bug, the
      tentative allocation is not shifted or unreserved in order to ensure
      the register count is higher to workaround the bug.

    .. note::

      This approach of using a tentative scratch V# and shifting the register
      numbers if used avoids having to perform register allocation a second
      time if the tentative V# is eliminated. This is more efficient and
      avoids the problem that the second register allocation may perform
      spilling which will fail as there is no longer a scratch V#.

When the kernel prolog code is being emitted it is known whether the scratch V#
described above is actually used. If it is, the prolog code must set it up by
copying the Private Segment Buffer to the scratch V# registers and then adding
the Private Segment Wavefront Offset to the queue base address in the V#. The
result is a V# with a base address pointing to the beginning of the wavefront
scratch backing memory.

The Private Segment Buffer is always requested, but the Private Segment
Wavefront Offset is only requested if it is used (see
:ref:`amdgpu-amdhsa-initial-kernel-execution-state`).

.. _amdgpu-amdhsa-memory-model:

Memory Model
~~~~~~~~~~~~

This section describes the mapping of the LLVM memory model onto AMDGPU machine
code (see :ref:`memmodel`).

The AMDGPU backend supports the memory synchronization scopes specified in
:ref:`amdgpu-memory-scopes`.

The code sequences used to implement the memory model specify the order of
instructions that a single thread must execute. The ``s_waitcnt`` and cache
management instructions such as ``buffer_wbinvl1_vol`` are defined with respect
to other memory instructions executed by the same thread. This allows them to be
moved earlier or later which can allow them to be combined with other instances
of the same instruction, or hoisted/sunk out of loops to improve performance.
Only the instructions related to the memory model are given; additional
``s_waitcnt`` instructions are required to ensure registers are defined before
being used. These may be able to be combined with the memory model ``s_waitcnt``
instructions as described above.

The AMDGPU backend supports the following memory models:

  HSA Memory Model [HSA]_
    The HSA memory model uses a single happens-before relation for all address
    spaces (see :ref:`amdgpu-address-spaces`).
  OpenCL Memory Model [OpenCL]_
    The OpenCL memory model which has separate happens-before relations for the
    global and local address spaces. Only a fence specifying both global and
    local address space, and seq_cst instructions join the relationships. Since
    the LLVM ``memfence`` instruction does not allow an address space to be
    specified the OpenCL fence has to conservatively assume both local and
    global address space was specified. However, optimizations can often be
    done to eliminate the additional ``s_waitcnt`` instructions when there are
    no intervening memory instructions which access the corresponding address
    space. The code sequences in the table indicate what can be omitted for the
    OpenCL memory. The target triple environment is used to determine if the
    source language is OpenCL (see :ref:`amdgpu-opencl`).

``ds/flat_load/store/atomic`` instructions to local memory are termed LDS
operations.

``buffer/global/flat_load/store/atomic`` instructions to global memory are
termed vector memory operations.

Private address space uses ``buffer_load/store`` using the scratch V#
(GFX6-GFX8), or ``scratch_load/store`` (GFX9-GFX11). Since only a single thread
is accessing the memory, atomic memory orderings are not meaningful, and all
accesses are treated as non-atomic.

Constant address space uses ``buffer/global_load`` instructions (or equivalent
scalar memory instructions). Since the constant address space contents do not
change during the execution of a kernel dispatch it is not legal to perform
stores, and atomic memory orderings are not meaningful, and all accesses are
treated as non-atomic.

A memory synchronization scope wider than work-group is not meaningful for the
group (LDS) address space and is treated as work-group.

The memory model does not support the region address space which is treated as
non-atomic.

Acquire memory ordering is not meaningful on store atomic instructions and is
treated as non-atomic.

Release memory ordering is not meaningful on load atomic instructions and is
treated a non-atomic.

Acquire-release memory ordering is not meaningful on load or store atomic
instructions and is treated as acquire and release respectively.

The memory order also adds the single thread optimization constraints defined in
table
:ref:`amdgpu-amdhsa-memory-model-single-thread-optimization-constraints-table`.

  .. table:: AMDHSA Memory Model Single Thread Optimization Constraints
     :name: amdgpu-amdhsa-memory-model-single-thread-optimization-constraints-table

     ============ ==============================================================
     LLVM Memory  Optimization Constraints
     Ordering
     ============ ==============================================================
     unordered    *none*
     monotonic    *none*
     acquire      - If a load atomic/atomicrmw then no following load/load
                    atomic/store/store atomic/atomicrmw/fence instruction can be
                    moved before the acquire.
                  - If a fence then same as load atomic, plus no preceding
                    associated fence-paired-atomic can be moved after the fence.
     release      - If a store atomic/atomicrmw then no preceding load/load
                    atomic/store/store atomic/atomicrmw/fence instruction can be
                    moved after the release.
                  - If a fence then same as store atomic, plus no following
                    associated fence-paired-atomic can be moved before the
                    fence.
     acq_rel      Same constraints as both acquire and release.
     seq_cst      - If a load atomic then same constraints as acquire, plus no
                    preceding sequentially consistent load atomic/store
                    atomic/atomicrmw/fence instruction can be moved after the
                    seq_cst.
                  - If a store atomic then the same constraints as release, plus
                    no following sequentially consistent load atomic/store
                    atomic/atomicrmw/fence instruction can be moved before the
                    seq_cst.
                  - If an atomicrmw/fence then same constraints as acq_rel.
     ============ ==============================================================

The code sequences used to implement the memory model are defined in the
following sections:

* :ref:`amdgpu-amdhsa-memory-model-gfx6-gfx9`
* :ref:`amdgpu-amdhsa-memory-model-gfx90a`
* :ref:`amdgpu-amdhsa-memory-model-gfx940`
* :ref:`amdgpu-amdhsa-memory-model-gfx10-gfx11`

.. _amdgpu-amdhsa-memory-model-gfx6-gfx9:

Memory Model GFX6-GFX9
++++++++++++++++++++++

For GFX6-GFX9:

* Each agent has multiple shader arrays (SA).
* Each SA has multiple compute units (CU).
* Each CU has multiple SIMDs that execute wavefronts.
* The wavefronts for a single work-group are executed in the same CU but may be
  executed by different SIMDs.
* Each CU has a single LDS memory shared by the wavefronts of the work-groups
  executing on it.
* All LDS operations of a CU are performed as wavefront wide operations in a
  global order and involve no caching. Completion is reported to a wavefront in
  execution order.
* The LDS memory has multiple request queues shared by the SIMDs of a
  CU. Therefore, the LDS operations performed by different wavefronts of a
  work-group can be reordered relative to each other, which can result in
  reordering the visibility of vector memory operations with respect to LDS
  operations of other wavefronts in the same work-group. A ``s_waitcnt
  lgkmcnt(0)`` is required to ensure synchronization between LDS operations and
  vector memory operations between wavefronts of a work-group, but not between
  operations performed by the same wavefront.
* The vector memory operations are performed as wavefront wide operations and
  completion is reported to a wavefront in execution order. The exception is
  that for GFX7-GFX9 ``flat_load/store/atomic`` instructions can report out of
  vector memory order if they access LDS memory, and out of LDS operation order
  if they access global memory.
* The vector memory operations access a single vector L1 cache shared by all
  SIMDs a CU. Therefore, no special action is required for coherence between the
  lanes of a single wavefront, or for coherence between wavefronts in the same
  work-group. A ``buffer_wbinvl1_vol`` is required for coherence between
  wavefronts executing in different work-groups as they may be executing on
  different CUs.
* The scalar memory operations access a scalar L1 cache shared by all wavefronts
  on a group of CUs. The scalar and vector L1 caches are not coherent. However,
  scalar operations are used in a restricted way so do not impact the memory
  model. See :ref:`amdgpu-amdhsa-memory-spaces`.
* The vector and scalar memory operations use an L2 cache shared by all CUs on
  the same agent.
* The L2 cache has independent channels to service disjoint ranges of virtual
  addresses.
* Each CU has a separate request queue per channel. Therefore, the vector and
  scalar memory operations performed by wavefronts executing in different
  work-groups (which may be executing on different CUs) of an agent can be
  reordered relative to each other. A ``s_waitcnt vmcnt(0)`` is required to
  ensure synchronization between vector memory operations of different CUs. It
  ensures a previous vector memory operation has completed before executing a
  subsequent vector memory or LDS operation and so can be used to meet the
  requirements of acquire and release.
* The L2 cache can be kept coherent with other agents on some targets, or ranges
  of virtual addresses can be set up to bypass it to ensure system coherence.

Scalar memory operations are only used to access memory that is proven to not
change during the execution of the kernel dispatch. This includes constant
address space and global address space for program scope ``const`` variables.
Therefore, the kernel machine code does not have to maintain the scalar cache to
ensure it is coherent with the vector caches. The scalar and vector caches are
invalidated between kernel dispatches by CP since constant address space data
may change between kernel dispatch executions. See
:ref:`amdgpu-amdhsa-memory-spaces`.

The one exception is if scalar writes are used to spill SGPR registers. In this
case the AMDGPU backend ensures the memory location used to spill is never
accessed by vector memory operations at the same time. If scalar writes are used
then a ``s_dcache_wb`` is inserted before the ``s_endpgm`` and before a function
return since the locations may be used for vector memory instructions by a
future wavefront that uses the same scratch area, or a function call that
creates a frame at the same address, respectively. There is no need for a
``s_dcache_inv`` as all scalar writes are write-before-read in the same thread.

For kernarg backing memory:

* CP invalidates the L1 cache at the start of each kernel dispatch.
* On dGPU the kernarg backing memory is allocated in host memory accessed as
  MTYPE UC (uncached) to avoid needing to invalidate the L2 cache. This also
  causes it to be treated as non-volatile and so is not invalidated by
  ``*_vol``.
* On APU the kernarg backing memory it is accessed as MTYPE CC (cache coherent)
  and so the L2 cache will be coherent with the CPU and other agents.

Scratch backing memory (which is used for the private address space) is accessed
with MTYPE NC_NV (non-coherent non-volatile). Since the private address space is
only accessed by a single thread, and is always write-before-read, there is
never a need to invalidate these entries from the L1 cache. Hence all cache
invalidates are done as ``*_vol`` to only invalidate the volatile cache lines.

The code sequences used to implement the memory model for GFX6-GFX9 are defined
in table :ref:`amdgpu-amdhsa-memory-model-code-sequences-gfx6-gfx9-table`.

  .. table:: AMDHSA Memory Model Code Sequences GFX6-GFX9
     :name: amdgpu-amdhsa-memory-model-code-sequences-gfx6-gfx9-table

     ============ ============ ============== ========== ================================
     LLVM Instr   LLVM Memory  LLVM Memory    AMDGPU     AMDGPU Machine Code
                  Ordering     Sync Scope     Address    GFX6-GFX9
                                              Space
     ============ ============ ============== ========== ================================
     **Non-Atomic**
     ------------------------------------------------------------------------------------
     load         *none*       *none*         - global   - !volatile & !nontemporal
                                              - generic
                                              - private    1. buffer/global/flat_load
                                              - constant
                                                         - !volatile & nontemporal

                                                           1. buffer/global/flat_load
                                                              glc=1 slc=1

                                                         - volatile

                                                           1. buffer/global/flat_load
                                                              glc=1
                                                           2. s_waitcnt vmcnt(0)

                                                            - Must happen before
                                                              any following volatile
                                                              global/generic
                                                              load/store.
                                                            - Ensures that
                                                              volatile
                                                              operations to
                                                              different
                                                              addresses will not
                                                              be reordered by
                                                              hardware.

     load         *none*       *none*         - local    1. ds_load
     store        *none*       *none*         - global   - !volatile & !nontemporal
                                              - generic
                                              - private    1. buffer/global/flat_store
                                              - constant
                                                         - !volatile & nontemporal

                                                           1. buffer/global/flat_store
                                                              glc=1 slc=1

                                                         - volatile

                                                           1. buffer/global/flat_store
                                                           2. s_waitcnt vmcnt(0)

                                                            - Must happen before
                                                              any following volatile
                                                              global/generic
                                                              load/store.
                                                            - Ensures that
                                                              volatile
                                                              operations to
                                                              different
                                                              addresses will not
                                                              be reordered by
                                                              hardware.

     store        *none*       *none*         - local    1. ds_store
     **Unordered Atomic**
     ------------------------------------------------------------------------------------
     load atomic  unordered    *any*          *any*      *Same as non-atomic*.
     store atomic unordered    *any*          *any*      *Same as non-atomic*.
     atomicrmw    unordered    *any*          *any*      *Same as monotonic atomic*.
     **Monotonic Atomic**
     ------------------------------------------------------------------------------------
     load atomic  monotonic    - singlethread - global   1. buffer/global/ds/flat_load
                               - wavefront    - local
                               - workgroup    - generic
     load atomic  monotonic    - agent        - global   1. buffer/global/flat_load
                               - system       - generic     glc=1
     store atomic monotonic    - singlethread - global   1. buffer/global/flat_store
                               - wavefront    - generic
                               - workgroup
                               - agent
                               - system
     store atomic monotonic    - singlethread - local    1. ds_store
                               - wavefront
                               - workgroup
     atomicrmw    monotonic    - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
                               - workgroup
                               - agent
                               - system
     atomicrmw    monotonic    - singlethread - local    1. ds_atomic
                               - wavefront
                               - workgroup
     **Acquire Atomic**
     ------------------------------------------------------------------------------------
     load atomic  acquire      - singlethread - global   1. buffer/global/ds/flat_load
                               - wavefront    - local
                                              - generic
     load atomic  acquire      - workgroup    - global   1. buffer/global_load
     load atomic  acquire      - workgroup    - local    1. ds/flat_load
                                              - generic  2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local load
                                                             atomic value being
                                                             acquired.

     load atomic  acquire      - agent        - global   1. buffer/global_load
                               - system                     glc=1
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the load
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale global data.

     load atomic  acquire      - agent        - generic  1. flat_load glc=1
                               - system                  2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If OpenCL omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the flat_load
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - singlethread - global   1. buffer/global/ds/flat_atomic
                               - wavefront    - local
                                              - generic
     atomicrmw    acquire      - workgroup    - global   1. buffer/global_atomic
     atomicrmw    acquire      - workgroup    - local    1. ds/flat_atomic
                                              - generic  2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local
                                                             atomicrmw value
                                                             being acquired.

     atomicrmw    acquire      - agent        - global   1. buffer/global_atomic
                               - system                  2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         3. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - agent        - generic  1. flat_atomic
                               - system                  2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         3. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     fence        acquire      - singlethread *none*     *none*
                               - wavefront
     fence        acquire      - workgroup    *none*     1. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit.
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - Must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the
                                                             value read by the
                                                             fence-paired-atomic.

     fence        acquire      - agent        *none*     1. s_waitcnt lgkmcnt(0) &
                               - system                     vmcnt(0)

                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures that the
                                                             fence-paired atomic
                                                             has completed
                                                             before invalidating
                                                             the
                                                             cache. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             fence-paired-atomic.

                                                         2. buffer_wbinvl1_vol

                                                           - Must happen before any
                                                             following global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     **Release Atomic**
     ------------------------------------------------------------------------------------
     store atomic release      - singlethread - global   1. buffer/global/ds/flat_store
                               - wavefront    - local
                                              - generic
     store atomic release      - workgroup    - global   1. s_waitcnt lgkmcnt(0)
                                              - generic
                                                           - If OpenCL, omit.
                                                           - Must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             store.
                                                           - Ensures that all
                                                             memory operations
                                                             to local have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         2. buffer/global/flat_store
     store atomic release      - workgroup    - local    1. ds_store
     store atomic release      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                               - system       - generic     vmcnt(0)

                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             store.
                                                           - Ensures that all
                                                             memory operations
                                                             to memory have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         2. buffer/global/flat_store
     atomicrmw    release      - singlethread - global   1. buffer/global/ds/flat_atomic
                               - wavefront    - local
                                              - generic
     atomicrmw    release      - workgroup    - global   1. s_waitcnt lgkmcnt(0)
                                              - generic
                                                           - If OpenCL, omit.
                                                           - Must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to local have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. buffer/global/flat_atomic
     atomicrmw    release      - workgroup    - local    1. ds_atomic
     atomicrmw    release      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                               - system       - generic     vmcnt(0)

                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global and local
                                                             have completed
                                                             before performing
                                                             the atomicrmw that
                                                             is being released.

                                                         2. buffer/global/flat_atomic
     fence        release      - singlethread *none*     *none*
                               - wavefront
     fence        release      - workgroup    *none*     1. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit.
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - Must happen after
                                                             any preceding
                                                             local/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Ensures that all
                                                             memory operations
                                                             to local have
                                                             completed before
                                                             performing the
                                                             following
                                                             fence-paired-atomic.

     fence        release      - agent        *none*     1. s_waitcnt lgkmcnt(0) &
                               - system                     vmcnt(0)

                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             following
                                                             fence-paired-atomic.

     **Acquire-Release Atomic**
     ------------------------------------------------------------------------------------
     atomicrmw    acq_rel      - singlethread - global   1. buffer/global/ds/flat_atomic
                               - wavefront    - local
                                              - generic
     atomicrmw    acq_rel      - workgroup    - global   1. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to local have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. buffer/global_atomic

     atomicrmw    acq_rel      - workgroup    - local    1. ds_atomic
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local load
                                                             atomic value being
                                                             acquired.

     atomicrmw    acq_rel      - workgroup    - generic  1. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to local have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. flat_atomic
                                                         3. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local load
                                                             atomic value being
                                                             acquired.

     atomicrmw    acq_rel      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                               - system                     vmcnt(0)

                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. buffer/global_atomic
                                                         3. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         4. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acq_rel      - agent        - generic  1. s_waitcnt lgkmcnt(0) &
                               - system                     vmcnt(0)

                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. flat_atomic
                                                         3. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         4. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     fence        acq_rel      - singlethread *none*     *none*
                               - wavefront
     fence        acq_rel      - workgroup    *none*     1. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit.
                                                           - However,
                                                             since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Must happen after
                                                             any preceding
                                                             local/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to local have
                                                             completed before
                                                             performing any
                                                             following global
                                                             memory operations.
                                                           - Ensures that the
                                                             preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             acquire-fence-paired-atomic)
                                                             has completed
                                                             before following
                                                             global memory
                                                             operations. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.
                                                           - Ensures that all
                                                             previous memory
                                                             operations have
                                                             completed before a
                                                             following
                                                             local/generic store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             release-fence-paired-atomic).
                                                             This satisfies the
                                                             requirements of
                                                             release.

     fence        acq_rel      - agent        *none*     1. s_waitcnt lgkmcnt(0) &
                               - system                     vmcnt(0)

                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures that the
                                                             preceding
                                                             global/local/generic
                                                             load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             acquire-fence-paired-atomic)
                                                             has completed
                                                             before invalidating
                                                             the cache. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.
                                                           - Ensures that all
                                                             previous memory
                                                             operations have
                                                             completed before a
                                                             following
                                                             global/local/generic
                                                             store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             release-fence-paired-atomic).
                                                             This satisfies the
                                                             requirements of
                                                             release.

                                                         2. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.

     **Sequential Consistent Atomic**
     ------------------------------------------------------------------------------------
     load atomic  seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    load atomic acquire,
                                              - generic  except must generate
                                                         all instructions even
                                                         for OpenCL.*
     load atomic  seq_cst      - workgroup    - global   1. s_waitcnt lgkmcnt(0)
                                              - generic

                                                           - Must
                                                             happen after
                                                             preceding
                                                             local/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             lgkmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - Ensures any
                                                             preceding
                                                             sequential
                                                             consistent local
                                                             memory instructions
                                                             have completed
                                                             before executing
                                                             this sequentially
                                                             consistent
                                                             instruction. This
                                                             prevents reordering
                                                             a seq_cst store
                                                             followed by a
                                                             seq_cst load. (Note
                                                             that seq_cst is
                                                             stronger than
                                                             acquire/release as
                                                             the reordering of
                                                             load acquire
                                                             followed by a store
                                                             release is
                                                             prevented by the
                                                             s_waitcnt of
                                                             the release, but
                                                             there is nothing
                                                             preventing a store
                                                             release followed by
                                                             load acquire from
                                                             completing out of
                                                             order. The s_waitcnt
                                                             could be placed after
                                                             seq_store or before
                                                             the seq_load. We
                                                             choose the load to
                                                             make the s_waitcnt be
                                                             as late as possible
                                                             so that the store
                                                             may have already
                                                             completed.)

                                                         2. *Following
                                                            instructions same as
                                                            corresponding load
                                                            atomic acquire,
                                                            except must generate
                                                            all instructions even
                                                            for OpenCL.*
     load atomic  seq_cst      - workgroup    - local    *Same as corresponding
                                                         load atomic acquire,
                                                         except must generate
                                                         all instructions even
                                                         for OpenCL.*

     load atomic  seq_cst      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                               - system       - generic     vmcnt(0)

                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0)
                                                             and s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             preceding
                                                             global/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             lgkmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             preceding
                                                             global/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             vmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - Ensures any
                                                             preceding
                                                             sequential
                                                             consistent global
                                                             memory instructions
                                                             have completed
                                                             before executing
                                                             this sequentially
                                                             consistent
                                                             instruction. This
                                                             prevents reordering
                                                             a seq_cst store
                                                             followed by a
                                                             seq_cst load. (Note
                                                             that seq_cst is
                                                             stronger than
                                                             acquire/release as
                                                             the reordering of
                                                             load acquire
                                                             followed by a store
                                                             release is
                                                             prevented by the
                                                             s_waitcnt of
                                                             the release, but
                                                             there is nothing
                                                             preventing a store
                                                             release followed by
                                                             load acquire from
                                                             completing out of
                                                             order. The s_waitcnt
                                                             could be placed after
                                                             seq_store or before
                                                             the seq_load. We
                                                             choose the load to
                                                             make the s_waitcnt be
                                                             as late as possible
                                                             so that the store
                                                             may have already
                                                             completed.)

                                                         2. *Following
                                                            instructions same as
                                                            corresponding load
                                                            atomic acquire,
                                                            except must generate
                                                            all instructions even
                                                            for OpenCL.*
     store atomic seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    store atomic release,
                               - workgroup    - generic  except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     atomicrmw    seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    atomicrmw acq_rel,
                               - workgroup    - generic  except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     fence        seq_cst      - singlethread *none*     *Same as corresponding
                               - wavefront               fence acq_rel,
                               - workgroup               except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     ============ ============ ============== ========== ================================

.. _amdgpu-amdhsa-memory-model-gfx90a:

Memory Model GFX90A
+++++++++++++++++++

For GFX90A:

* Each agent has multiple shader arrays (SA).
* Each SA has multiple compute units (CU).
* Each CU has multiple SIMDs that execute wavefronts.
* The wavefronts for a single work-group are executed in the same CU but may be
  executed by different SIMDs. The exception is when in tgsplit execution mode
  when the wavefronts may be executed by different SIMDs in different CUs.
* Each CU has a single LDS memory shared by the wavefronts of the work-groups
  executing on it. The exception is when in tgsplit execution mode when no LDS
  is allocated as wavefronts of the same work-group can be in different CUs.
* All LDS operations of a CU are performed as wavefront wide operations in a
  global order and involve no caching. Completion is reported to a wavefront in
  execution order.
* The LDS memory has multiple request queues shared by the SIMDs of a
  CU. Therefore, the LDS operations performed by different wavefronts of a
  work-group can be reordered relative to each other, which can result in
  reordering the visibility of vector memory operations with respect to LDS
  operations of other wavefronts in the same work-group. A ``s_waitcnt
  lgkmcnt(0)`` is required to ensure synchronization between LDS operations and
  vector memory operations between wavefronts of a work-group, but not between
  operations performed by the same wavefront.
* The vector memory operations are performed as wavefront wide operations and
  completion is reported to a wavefront in execution order. The exception is
  that ``flat_load/store/atomic`` instructions can report out of vector memory
  order if they access LDS memory, and out of LDS operation order if they access
  global memory.
* The vector memory operations access a single vector L1 cache shared by all
  SIMDs a CU. Therefore:

  * No special action is required for coherence between the lanes of a single
    wavefront.

  * No special action is required for coherence between wavefronts in the same
    work-group since they execute on the same CU. The exception is when in
    tgsplit execution mode as wavefronts of the same work-group can be in
    different CUs and so a ``buffer_wbinvl1_vol`` is required as described in
    the following item.

  * A ``buffer_wbinvl1_vol`` is required for coherence between wavefronts
    executing in different work-groups as they may be executing on different
    CUs.

* The scalar memory operations access a scalar L1 cache shared by all wavefronts
  on a group of CUs. The scalar and vector L1 caches are not coherent. However,
  scalar operations are used in a restricted way so do not impact the memory
  model. See :ref:`amdgpu-amdhsa-memory-spaces`.
* The vector and scalar memory operations use an L2 cache shared by all CUs on
  the same agent.

  * The L2 cache has independent channels to service disjoint ranges of virtual
    addresses.
  * Each CU has a separate request queue per channel. Therefore, the vector and
    scalar memory operations performed by wavefronts executing in different
    work-groups (which may be executing on different CUs), or the same
    work-group if executing in tgsplit mode, of an agent can be reordered
    relative to each other. A ``s_waitcnt vmcnt(0)`` is required to ensure
    synchronization between vector memory operations of different CUs. It
    ensures a previous vector memory operation has completed before executing a
    subsequent vector memory or LDS operation and so can be used to meet the
    requirements of acquire and release.
  * The L2 cache of one agent can be kept coherent with other agents by:
    using the MTYPE RW (read-write) or MTYPE CC (cache-coherent) with the PTE
    C-bit for memory local to the L2; and using the MTYPE NC (non-coherent) with
    the PTE C-bit set or MTYPE UC (uncached) for memory not local to the L2.

    * Any local memory cache lines will be automatically invalidated by writes
      from CUs associated with other L2 caches, or writes from the CPU, due to
      the cache probe caused by coherent requests. Coherent requests are caused
      by GPU accesses to pages with the PTE C-bit set, by CPU accesses over
      XGMI, and by PCIe requests that are configured to be coherent requests.
    * XGMI accesses from the CPU to local memory may be cached on the CPU.
      Subsequent access from the GPU will automatically invalidate or writeback
      the CPU cache due to the L2 probe filter and and the PTE C-bit being set.
    * Since all work-groups on the same agent share the same L2, no L2
      invalidation or writeback is required for coherence.
    * To ensure coherence of local and remote memory writes of work-groups in
      different agents a ``buffer_wbl2`` is required. It will writeback dirty L2
      cache lines of MTYPE RW (used for local coarse grain memory) and MTYPE NC
      ()used for remote coarse grain memory). Note that MTYPE CC (used for local
      fine grain memory) causes write through to DRAM, and MTYPE UC (used for
      remote fine grain memory) bypasses the L2, so both will never result in
      dirty L2 cache lines.
    * To ensure coherence of local and remote memory reads of work-groups in
      different agents a ``buffer_invl2`` is required. It will invalidate L2
      cache lines with MTYPE NC (used for remote coarse grain memory). Note that
      MTYPE CC (used for local fine grain memory) and MTYPE RW (used for local
      coarse memory) cause local reads to be invalidated by remote writes with
      with the PTE C-bit so these cache lines are not invalidated. Note that
      MTYPE UC (used for remote fine grain memory) bypasses the L2, so will
      never result in L2 cache lines that need to be invalidated.

  * PCIe access from the GPU to the CPU memory is kept coherent by using the
    MTYPE UC (uncached) which bypasses the L2.

Scalar memory operations are only used to access memory that is proven to not
change during the execution of the kernel dispatch. This includes constant
address space and global address space for program scope ``const`` variables.
Therefore, the kernel machine code does not have to maintain the scalar cache to
ensure it is coherent with the vector caches. The scalar and vector caches are
invalidated between kernel dispatches by CP since constant address space data
may change between kernel dispatch executions. See
:ref:`amdgpu-amdhsa-memory-spaces`.

The one exception is if scalar writes are used to spill SGPR registers. In this
case the AMDGPU backend ensures the memory location used to spill is never
accessed by vector memory operations at the same time. If scalar writes are used
then a ``s_dcache_wb`` is inserted before the ``s_endpgm`` and before a function
return since the locations may be used for vector memory instructions by a
future wavefront that uses the same scratch area, or a function call that
creates a frame at the same address, respectively. There is no need for a
``s_dcache_inv`` as all scalar writes are write-before-read in the same thread.

For kernarg backing memory:

* CP invalidates the L1 cache at the start of each kernel dispatch.
* On dGPU over XGMI or PCIe the kernarg backing memory is allocated in host
  memory accessed as MTYPE UC (uncached) to avoid needing to invalidate the L2
  cache. This also causes it to be treated as non-volatile and so is not
  invalidated by ``*_vol``.
* On APU the kernarg backing memory is accessed as MTYPE CC (cache coherent) and
  so the L2 cache will be coherent with the CPU and other agents.

Scratch backing memory (which is used for the private address space) is accessed
with MTYPE NC_NV (non-coherent non-volatile). Since the private address space is
only accessed by a single thread, and is always write-before-read, there is
never a need to invalidate these entries from the L1 cache. Hence all cache
invalidates are done as ``*_vol`` to only invalidate the volatile cache lines.

The code sequences used to implement the memory model for GFX90A are defined
in table :ref:`amdgpu-amdhsa-memory-model-code-sequences-gfx90a-table`.

  .. table:: AMDHSA Memory Model Code Sequences GFX90A
     :name: amdgpu-amdhsa-memory-model-code-sequences-gfx90a-table

     ============ ============ ============== ========== ================================
     LLVM Instr   LLVM Memory  LLVM Memory    AMDGPU     AMDGPU Machine Code
                  Ordering     Sync Scope     Address    GFX90A
                                              Space
     ============ ============ ============== ========== ================================
     **Non-Atomic**
     ------------------------------------------------------------------------------------
     load         *none*       *none*         - global   - !volatile & !nontemporal
                                              - generic
                                              - private    1. buffer/global/flat_load
                                              - constant
                                                         - !volatile & nontemporal

                                                           1. buffer/global/flat_load
                                                              glc=1 slc=1

                                                         - volatile

                                                           1. buffer/global/flat_load
                                                              glc=1
                                                           2. s_waitcnt vmcnt(0)

                                                            - Must happen before
                                                              any following volatile
                                                              global/generic
                                                              load/store.
                                                            - Ensures that
                                                              volatile
                                                              operations to
                                                              different
                                                              addresses will not
                                                              be reordered by
                                                              hardware.

     load         *none*       *none*         - local    1. ds_load
     store        *none*       *none*         - global   - !volatile & !nontemporal
                                              - generic
                                              - private    1. buffer/global/flat_store
                                              - constant
                                                         - !volatile & nontemporal

                                                           1. buffer/global/flat_store
                                                              glc=1 slc=1

                                                         - volatile

                                                           1. buffer/global/flat_store
                                                           2. s_waitcnt vmcnt(0)

                                                            - Must happen before
                                                              any following volatile
                                                              global/generic
                                                              load/store.
                                                            - Ensures that
                                                              volatile
                                                              operations to
                                                              different
                                                              addresses will not
                                                              be reordered by
                                                              hardware.

     store        *none*       *none*         - local    1. ds_store
     **Unordered Atomic**
     ------------------------------------------------------------------------------------
     load atomic  unordered    *any*          *any*      *Same as non-atomic*.
     store atomic unordered    *any*          *any*      *Same as non-atomic*.
     atomicrmw    unordered    *any*          *any*      *Same as monotonic atomic*.
     **Monotonic Atomic**
     ------------------------------------------------------------------------------------
     load atomic  monotonic    - singlethread - global   1. buffer/global/flat_load
                               - wavefront    - generic
     load atomic  monotonic    - workgroup    - global   1. buffer/global/flat_load
                                              - generic     glc=1

                                                           - If not TgSplit execution
                                                             mode, omit glc=1.

     load atomic  monotonic    - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                               - workgroup               be used.*

                                                         1. ds_load
     load atomic  monotonic    - agent        - global   1. buffer/global/flat_load
                                              - generic     glc=1
     load atomic  monotonic    - system       - global   1. buffer/global/flat_load
                                              - generic     glc=1
     store atomic monotonic    - singlethread - global   1. buffer/global/flat_store
                               - wavefront    - generic
                               - workgroup
                               - agent
     store atomic monotonic    - system       - global   1. buffer/global/flat_store
                                              - generic
     store atomic monotonic    - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                               - workgroup               be used.*

                                                         1. ds_store
     atomicrmw    monotonic    - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
                               - workgroup
                               - agent
     atomicrmw    monotonic    - system       - global   1. buffer/global/flat_atomic
                                              - generic
     atomicrmw    monotonic    - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                               - workgroup               be used.*

                                                         1. ds_atomic
     **Acquire Atomic**
     ------------------------------------------------------------------------------------
     load atomic  acquire      - singlethread - global   1. buffer/global/ds/flat_load
                               - wavefront    - local
                                              - generic
     load atomic  acquire      - workgroup    - global   1. buffer/global_load glc=1

                                                           - If not TgSplit execution
                                                             mode, omit glc=1.

                                                         2. s_waitcnt vmcnt(0)

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before the
                                                             following buffer_wbinvl1_vol.

                                                         3. buffer_wbinvl1_vol

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     load atomic  acquire      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_load
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local load
                                                             atomic value being
                                                             acquired.

     load atomic  acquire      - workgroup    - generic  1. flat_load glc=1

                                                           - If not TgSplit execution
                                                             mode, omit glc=1.

                                                         2. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit lgkmcnt(0).
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol and any
                                                             following global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local load
                                                             atomic value being
                                                             acquired.

                                                         3. buffer_wbinvl1_vol

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     load atomic  acquire      - agent        - global   1. buffer/global_load
                                                            glc=1
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the load
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale global data.

     load atomic  acquire      - system       - global   1. buffer/global/flat_load
                                                            glc=1
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following buffer_invl2 and
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the load
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_invl2;
                                                            buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale L1 global data,
                                                             nor see stale L2 MTYPE
                                                             NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale in L2 due to
                                                             the memory probes.

     load atomic  acquire      - agent        - generic  1. flat_load glc=1
                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the flat_load
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     load atomic  acquire      - system       - generic  1. flat_load glc=1
                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_invl2 and
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the flat_load
                                                             has completed
                                                             before invalidating
                                                             the caches.

                                                         3. buffer_invl2;
                                                            buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale L1 global data,
                                                             nor see stale L2 MTYPE
                                                             NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale in L2 due to
                                                             the memory probes.

     atomicrmw    acquire      - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
     atomicrmw    acquire      - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                                                         be used.*

                                                         1. ds_atomic
     atomicrmw    acquire      - workgroup    - global   1. buffer/global_atomic
                                                         2. s_waitcnt vmcnt(0)

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before the
                                                             following buffer_wbinvl1_vol.
                                                           - Ensures the atomicrmw
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_wbinvl1_vol

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_atomic
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local
                                                             atomicrmw value
                                                             being acquired.

     atomicrmw    acquire      - workgroup    - generic  1. flat_atomic
                                                         2. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit lgkmcnt(0).
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol and
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local
                                                             atomicrmw value
                                                             being acquired.

                                                         3. buffer_wbinvl1_vol

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acquire      - agent        - global   1. buffer/global_atomic
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         3. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - system       - global   1. buffer/global_atomic
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following buffer_invl2 and
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         3. buffer_invl2;
                                                            buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale L1 global data,
                                                             nor see stale L2 MTYPE
                                                             NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale in L2 due to
                                                             the memory probes.

     atomicrmw    acquire      - agent        - generic  1. flat_atomic
                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         3. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - system       - generic  1. flat_atomic
                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_invl2 and
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         3. buffer_invl2;
                                                            buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale L1 global data,
                                                             nor see stale L2 MTYPE
                                                             NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale in L2 due to
                                                             the memory probes.

     fence        acquire      - singlethread *none*     *none*
                               - wavefront
     fence        acquire      - workgroup    *none*     1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/
                                                             atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol and
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the
                                                             value read by the
                                                             fence-paired-atomic.

                                                         2. buffer_wbinvl1_vol

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     fence        acquire      - agent        *none*     1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures that the
                                                             fence-paired atomic
                                                             has completed
                                                             before invalidating
                                                             the
                                                             cache. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             fence-paired-atomic.

                                                         2. buffer_wbinvl1_vol

                                                           - Must happen before any
                                                             following global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     fence        acquire      - system       *none*     1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following buffer_invl2 and
                                                             buffer_wbinvl1_vol.
                                                           - Ensures that the
                                                             fence-paired atomic
                                                             has completed
                                                             before invalidating
                                                             the
                                                             cache. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             fence-paired-atomic.

                                                         2. buffer_invl2;
                                                            buffer_wbinvl1_vol

                                                           - Must happen before any
                                                             following global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale L1 global data,
                                                             nor see stale L2 MTYPE
                                                             NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale in L2 due to
                                                             the memory probes.
     **Release Atomic**
     ------------------------------------------------------------------------------------
     store atomic release      - singlethread - global   1. buffer/global/flat_store
                               - wavefront    - generic
     store atomic release      - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                                                         be used.*

                                                         1. ds_store
     store atomic release      - workgroup    - global   1. s_waitcnt lgkm/vmcnt(0)
                                              - generic
                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit lgkmcnt(0).
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             store.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         2. buffer/global/flat_store
     store atomic release      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_store
     store atomic release      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                                              - generic     vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             store.
                                                           - Ensures that all
                                                             memory operations
                                                             to memory have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         2. buffer/global/flat_store
     store atomic release      - system       - global   1. buffer_wbl2
                                              - generic
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after any
                                                             preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after any
                                                             preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             store.
                                                           - Ensures that all
                                                             memory operations
                                                             to memory and the L2
                                                             writeback have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         3. buffer/global/flat_store
     atomicrmw    release      - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
     atomicrmw    release      - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                                                         be used.*

                                                         1. ds_atomic
     atomicrmw    release      - workgroup    - global   1. s_waitcnt lgkm/vmcnt(0)
                                              - generic
                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. buffer/global/flat_atomic
     atomicrmw    release      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_atomic
     atomicrmw    release      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                                              - generic     vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global and local
                                                             have completed
                                                             before performing
                                                             the atomicrmw that
                                                             is being released.

                                                         2. buffer/global/flat_atomic
     atomicrmw    release      - system       - global   1. buffer_wbl2
                                              - generic
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to memory and the L2
                                                             writeback have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         3. buffer/global/flat_atomic
     fence        release      - singlethread *none*     *none*
                               - wavefront
     fence        release      - workgroup    *none*     1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             following
                                                             fence-paired-atomic.

     fence        release      - agent        *none*     1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             following
                                                             fence-paired-atomic.

     fence        release      - system       *none*     1. buffer_wbl2

                                                           - If OpenCL and
                                                             address space is
                                                             local, omit.
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             following
                                                             fence-paired-atomic.

     **Acquire-Release Atomic**
     ------------------------------------------------------------------------------------
     atomicrmw    acq_rel      - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
     atomicrmw    acq_rel      - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                                                         be used.*

                                                         1. ds_atomic
     atomicrmw    acq_rel      - workgroup    - global   1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. buffer/global_atomic
                                                         3. s_waitcnt vmcnt(0)

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the
                                                             atomicrmw value
                                                             being acquired.

                                                         4. buffer_wbinvl1_vol

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acq_rel      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_atomic
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local load
                                                             atomic value being
                                                             acquired.

     atomicrmw    acq_rel      - workgroup    - generic  1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. flat_atomic
                                                         3. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If not TgSplit execution
                                                             mode, omit vmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol and
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local load
                                                             atomic value being
                                                             acquired.

                                                         3. buffer_wbinvl1_vol

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acq_rel      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. buffer/global_atomic
                                                         3. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         4. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acq_rel      - system       - global   1. buffer_wbl2

                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global and L2 writeback
                                                             have completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         3. buffer/global_atomic
                                                         4. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following buffer_invl2 and
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         5. buffer_invl2;
                                                            buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale L1 global data,
                                                             nor see stale L2 MTYPE
                                                             NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale in L2 due to
                                                             the memory probes.

     atomicrmw    acq_rel      - agent        - generic  1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. flat_atomic
                                                         3. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         4. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acq_rel      - system       - generic  1. buffer_wbl2

                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global and L2 writeback
                                                             have completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         3. flat_atomic
                                                         4. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following buffer_invl2 and
                                                             buffer_wbinvl1_vol.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         5. buffer_invl2;
                                                            buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale L1 global data,
                                                             nor see stale L2 MTYPE
                                                             NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale in L2 due to
                                                             the memory probes.

     fence        acq_rel      - singlethread *none*     *none*
                               - wavefront
     fence        acq_rel      - workgroup    *none*     1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However,
                                                             since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing any
                                                             following global
                                                             memory operations.
                                                           - Ensures that the
                                                             preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             acquire-fence-paired-atomic)
                                                             has completed
                                                             before following
                                                             global memory
                                                             operations. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.
                                                           - Ensures that all
                                                             previous memory
                                                             operations have
                                                             completed before a
                                                             following
                                                             local/generic store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             release-fence-paired-atomic).
                                                             This satisfies the
                                                             requirements of
                                                             release.
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures that the
                                                             acquire-fence-paired
                                                             atomic has completed
                                                             before invalidating
                                                             the
                                                             cache. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             acquire-fence-paired-atomic.

                                                         2. buffer_wbinvl1_vol

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     fence        acq_rel      - agent        *none*     1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             buffer_wbinvl1_vol.
                                                           - Ensures that the
                                                             preceding
                                                             global/local/generic
                                                             load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             acquire-fence-paired-atomic)
                                                             has completed
                                                             before invalidating
                                                             the cache. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.
                                                           - Ensures that all
                                                             previous memory
                                                             operations have
                                                             completed before a
                                                             following
                                                             global/local/generic
                                                             store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             release-fence-paired-atomic).
                                                             This satisfies the
                                                             requirements of
                                                             release.

                                                         2. buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.

     fence        acq_rel      - system       *none*     1. buffer_wbl2

                                                           - If OpenCL and
                                                             address space is
                                                             local, omit.
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following buffer_invl2 and
                                                             buffer_wbinvl1_vol.
                                                           - Ensures that the
                                                             preceding
                                                             global/local/generic
                                                             load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             acquire-fence-paired-atomic)
                                                             has completed
                                                             before invalidating
                                                             the cache. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.
                                                           - Ensures that all
                                                             previous memory
                                                             operations have
                                                             completed before a
                                                             following
                                                             global/local/generic
                                                             store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             release-fence-paired-atomic).
                                                             This satisfies the
                                                             requirements of
                                                             release.

                                                         3.  buffer_invl2;
                                                             buffer_wbinvl1_vol

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale L1 global data,
                                                             nor see stale L2 MTYPE
                                                             NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale in L2 due to
                                                             the memory probes.

     **Sequential Consistent Atomic**
     ------------------------------------------------------------------------------------
     load atomic  seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    load atomic acquire,
                                              - generic  except must generate
                                                         all instructions even
                                                         for OpenCL.*
     load atomic  seq_cst      - workgroup    - global   1. s_waitcnt lgkm/vmcnt(0)
                                              - generic
                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - s_waitcnt lgkmcnt(0) must
                                                             happen after
                                                             preceding
                                                             local/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             lgkmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             preceding
                                                             global/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             vmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - Ensures any
                                                             preceding
                                                             sequential
                                                             consistent global/local
                                                             memory instructions
                                                             have completed
                                                             before executing
                                                             this sequentially
                                                             consistent
                                                             instruction. This
                                                             prevents reordering
                                                             a seq_cst store
                                                             followed by a
                                                             seq_cst load. (Note
                                                             that seq_cst is
                                                             stronger than
                                                             acquire/release as
                                                             the reordering of
                                                             load acquire
                                                             followed by a store
                                                             release is
                                                             prevented by the
                                                             s_waitcnt of
                                                             the release, but
                                                             there is nothing
                                                             preventing a store
                                                             release followed by
                                                             load acquire from
                                                             completing out of
                                                             order. The s_waitcnt
                                                             could be placed after
                                                             seq_store or before
                                                             the seq_load. We
                                                             choose the load to
                                                             make the s_waitcnt be
                                                             as late as possible
                                                             so that the store
                                                             may have already
                                                             completed.)

                                                         2. *Following
                                                            instructions same as
                                                            corresponding load
                                                            atomic acquire,
                                                            except must generate
                                                            all instructions even
                                                            for OpenCL.*
     load atomic  seq_cst      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         *Same as corresponding
                                                         load atomic acquire,
                                                         except must generate
                                                         all instructions even
                                                         for OpenCL.*

     load atomic  seq_cst      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                               - system       - generic     vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0)
                                                             and s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             preceding
                                                             global/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             lgkmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             preceding
                                                             global/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             vmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - Ensures any
                                                             preceding
                                                             sequential
                                                             consistent global
                                                             memory instructions
                                                             have completed
                                                             before executing
                                                             this sequentially
                                                             consistent
                                                             instruction. This
                                                             prevents reordering
                                                             a seq_cst store
                                                             followed by a
                                                             seq_cst load. (Note
                                                             that seq_cst is
                                                             stronger than
                                                             acquire/release as
                                                             the reordering of
                                                             load acquire
                                                             followed by a store
                                                             release is
                                                             prevented by the
                                                             s_waitcnt of
                                                             the release, but
                                                             there is nothing
                                                             preventing a store
                                                             release followed by
                                                             load acquire from
                                                             completing out of
                                                             order. The s_waitcnt
                                                             could be placed after
                                                             seq_store or before
                                                             the seq_load. We
                                                             choose the load to
                                                             make the s_waitcnt be
                                                             as late as possible
                                                             so that the store
                                                             may have already
                                                             completed.)

                                                         2. *Following
                                                            instructions same as
                                                            corresponding load
                                                            atomic acquire,
                                                            except must generate
                                                            all instructions even
                                                            for OpenCL.*
     store atomic seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    store atomic release,
                               - workgroup    - generic  except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     atomicrmw    seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    atomicrmw acq_rel,
                               - workgroup    - generic  except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     fence        seq_cst      - singlethread *none*     *Same as corresponding
                               - wavefront               fence acq_rel,
                               - workgroup               except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     ============ ============ ============== ========== ================================

.. _amdgpu-amdhsa-memory-model-gfx940:

Memory Model GFX940
+++++++++++++++++++

For GFX940:

* Each agent has multiple shader arrays (SA).
* Each SA has multiple compute units (CU).
* Each CU has multiple SIMDs that execute wavefronts.
* The wavefronts for a single work-group are executed in the same CU but may be
  executed by different SIMDs. The exception is when in tgsplit execution mode
  when the wavefronts may be executed by different SIMDs in different CUs.
* Each CU has a single LDS memory shared by the wavefronts of the work-groups
  executing on it. The exception is when in tgsplit execution mode when no LDS
  is allocated as wavefronts of the same work-group can be in different CUs.
* All LDS operations of a CU are performed as wavefront wide operations in a
  global order and involve no caching. Completion is reported to a wavefront in
  execution order.
* The LDS memory has multiple request queues shared by the SIMDs of a
  CU. Therefore, the LDS operations performed by different wavefronts of a
  work-group can be reordered relative to each other, which can result in
  reordering the visibility of vector memory operations with respect to LDS
  operations of other wavefronts in the same work-group. A ``s_waitcnt
  lgkmcnt(0)`` is required to ensure synchronization between LDS operations and
  vector memory operations between wavefronts of a work-group, but not between
  operations performed by the same wavefront.
* The vector memory operations are performed as wavefront wide operations and
  completion is reported to a wavefront in execution order. The exception is
  that ``flat_load/store/atomic`` instructions can report out of vector memory
  order if they access LDS memory, and out of LDS operation order if they access
  global memory.
* The vector memory operations access a single vector L1 cache shared by all
  SIMDs a CU. Therefore:

  * No special action is required for coherence between the lanes of a single
    wavefront.

  * No special action is required for coherence between wavefronts in the same
    work-group since they execute on the same CU. The exception is when in
    tgsplit execution mode as wavefronts of the same work-group can be in
    different CUs and so a ``buffer_inv sc0`` is required which will invalidate
    the L1 cache.

  * A ``buffer_inv sc0`` is required to invalidate the L1 cache for coherence
    between wavefronts executing in different work-groups as they may be
    executing on different CUs.

  * Atomic read-modify-write instructions implicitly bypass the L1 cache.
    Therefore, they do not use the sc0 bit for coherence and instead use it to
    indicate if the instruction returns the original value being updated. They
    do use sc1 to indicate system or agent scope coherence.

* The scalar memory operations access a scalar L1 cache shared by all wavefronts
  on a group of CUs. The scalar and vector L1 caches are not coherent. However,
  scalar operations are used in a restricted way so do not impact the memory
  model. See :ref:`amdgpu-amdhsa-memory-spaces`.
* The vector and scalar memory operations use an L2 cache.

  * The gfx940 can be configured as a number of smaller agents with each having
    a single L2 shared by all CUs on the same agent, or as fewer (possibly one)
    larger agents with groups of CUs on each agent each sharing separate L2
    caches.
  * The L2 cache has independent channels to service disjoint ranges of virtual
    addresses.
  * Each CU has a separate request queue per channel for its associated L2.
    Therefore, the vector and scalar memory operations performed by wavefronts
    executing with different L1 caches and the same L2 cache can be reordered
    relative to each other.
  * A ``s_waitcnt vmcnt(0)`` is required to ensure synchronization between
    vector memory operations of different CUs. It ensures a previous vector
    memory operation has completed before executing a subsequent vector memory
    or LDS operation and so can be used to meet the requirements of acquire and
    release.
  * An L2 cache can be kept coherent with other L2 caches by using the MTYPE RW
    (read-write) for memory local to the L2, and MTYPE NC (non-coherent) with
    the PTE C-bit set for memory not local to the L2.

    * Any local memory cache lines will be automatically invalidated by writes
      from CUs associated with other L2 caches, or writes from the CPU, due to
      the cache probe caused by the PTE C-bit.
    * XGMI accesses from the CPU to local memory may be cached on the CPU.
      Subsequent access from the GPU will automatically invalidate or writeback
      the CPU cache due to the L2 probe filter.
    * To ensure coherence of local memory writes of CUs with different L1 caches
      in the same agent a ``buffer_wbl2`` is required. It does nothing if the
      agent is configured to have a single L2, or will writeback dirty L2 cache
      lines if configured to have multiple L2 caches.
    * To ensure coherence of local memory writes of CUs in different agents a
      ``buffer_wbl2 sc1`` is required. It will writeback dirty L2 cache lines.
    * To ensure coherence of local memory reads of CUs with different L1 caches
      in the same agent a ``buffer_inv sc1`` is required. It does nothing if the
      agent is configured to have a single L2, or will invalidate non-local L2
      cache lines if configured to have multiple L2 caches.
    * To ensure coherence of local memory reads of CUs in different agents a
      ``buffer_inv sc0 sc1`` is required. It will invalidate non-local L2 cache
      lines if configured to have multiple L2 caches.

  * PCIe access from the GPU to the CPU can be kept coherent by using the MTYPE
    UC (uncached) which bypasses the L2.

Scalar memory operations are only used to access memory that is proven to not
change during the execution of the kernel dispatch. This includes constant
address space and global address space for program scope ``const`` variables.
Therefore, the kernel machine code does not have to maintain the scalar cache to
ensure it is coherent with the vector caches. The scalar and vector caches are
invalidated between kernel dispatches by CP since constant address space data
may change between kernel dispatch executions. See
:ref:`amdgpu-amdhsa-memory-spaces`.

The one exception is if scalar writes are used to spill SGPR registers. In this
case the AMDGPU backend ensures the memory location used to spill is never
accessed by vector memory operations at the same time. If scalar writes are used
then a ``s_dcache_wb`` is inserted before the ``s_endpgm`` and before a function
return since the locations may be used for vector memory instructions by a
future wavefront that uses the same scratch area, or a function call that
creates a frame at the same address, respectively. There is no need for a
``s_dcache_inv`` as all scalar writes are write-before-read in the same thread.

For kernarg backing memory:

* CP invalidates the L1 cache at the start of each kernel dispatch.
* On dGPU over XGMI or PCIe the kernarg backing memory is allocated in host
  memory accessed as MTYPE UC (uncached) to avoid needing to invalidate the L2
  cache. This also causes it to be treated as non-volatile and so is not
  invalidated by ``*_vol``.
* On APU the kernarg backing memory is accessed as MTYPE CC (cache coherent) and
  so the L2 cache will be coherent with the CPU and other agents.

Scratch backing memory (which is used for the private address space) is accessed
with MTYPE NC_NV (non-coherent non-volatile). Since the private address space is
only accessed by a single thread, and is always write-before-read, there is
never a need to invalidate these entries from the L1 cache. Hence all cache
invalidates are done as ``*_vol`` to only invalidate the volatile cache lines.

The code sequences used to implement the memory model for GFX940 are defined
in table :ref:`amdgpu-amdhsa-memory-model-code-sequences-gfx940-table`.

  .. table:: AMDHSA Memory Model Code Sequences GFX940
     :name: amdgpu-amdhsa-memory-model-code-sequences-gfx940-table

     ============ ============ ============== ========== ================================
     LLVM Instr   LLVM Memory  LLVM Memory    AMDGPU     AMDGPU Machine Code
                  Ordering     Sync Scope     Address    GFX940
                                              Space
     ============ ============ ============== ========== ================================
     **Non-Atomic**
     ------------------------------------------------------------------------------------
     load         *none*       *none*         - global   - !volatile & !nontemporal
                                              - generic
                                              - private    1. buffer/global/flat_load
                                              - constant
                                                         - !volatile & nontemporal

                                                           1. buffer/global/flat_load
                                                              nt=1

                                                         - volatile

                                                           1. buffer/global/flat_load
                                                              sc0=1 sc1=1
                                                           2. s_waitcnt vmcnt(0)

                                                            - Must happen before
                                                              any following volatile
                                                              global/generic
                                                              load/store.
                                                            - Ensures that
                                                              volatile
                                                              operations to
                                                              different
                                                              addresses will not
                                                              be reordered by
                                                              hardware.

     load         *none*       *none*         - local    1. ds_load
     store        *none*       *none*         - global   - !volatile & !nontemporal
                                              - generic
                                              - private    1. buffer/global/flat_store
                                              - constant
                                                         - !volatile & nontemporal

                                                           1. buffer/global/flat_store
                                                              nt=1

                                                         - volatile

                                                           1. buffer/global/flat_store
                                                              sc0=1 sc1=1
                                                           2. s_waitcnt vmcnt(0)

                                                            - Must happen before
                                                              any following volatile
                                                              global/generic
                                                              load/store.
                                                            - Ensures that
                                                              volatile
                                                              operations to
                                                              different
                                                              addresses will not
                                                              be reordered by
                                                              hardware.

     store        *none*       *none*         - local    1. ds_store
     **Unordered Atomic**
     ------------------------------------------------------------------------------------
     load atomic  unordered    *any*          *any*      *Same as non-atomic*.
     store atomic unordered    *any*          *any*      *Same as non-atomic*.
     atomicrmw    unordered    *any*          *any*      *Same as monotonic atomic*.
     **Monotonic Atomic**
     ------------------------------------------------------------------------------------
     load atomic  monotonic    - singlethread - global   1. buffer/global/flat_load
                               - wavefront    - generic
     load atomic  monotonic    - workgroup    - global   1. buffer/global/flat_load
                                              - generic     sc0=1
     load atomic  monotonic    - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                               - workgroup               be used.*

                                                         1. ds_load
     load atomic  monotonic    - agent        - global   1. buffer/global/flat_load
                                              - generic     sc1=1
     load atomic  monotonic    - system       - global   1. buffer/global/flat_load
                                              - generic     sc0=1 sc1=1
     store atomic monotonic    - singlethread - global   1. buffer/global/flat_store
                               - wavefront    - generic
     store atomic monotonic    - workgroup    - global   1. buffer/global/flat_store
                                              - generic     sc0=1
     store atomic monotonic    - agent        - global   1. buffer/global/flat_store
                                              - generic     sc1=1
     store atomic monotonic    - system       - global   1. buffer/global/flat_store
                                              - generic     sc0=1 sc1=1
     store atomic monotonic    - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                               - workgroup               be used.*

                                                         1. ds_store
     atomicrmw    monotonic    - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
                               - workgroup
                               - agent
     atomicrmw    monotonic    - system       - global   1. buffer/global/flat_atomic
                                              - generic     sc1=1
     atomicrmw    monotonic    - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                               - workgroup               be used.*

                                                         1. ds_atomic
     **Acquire Atomic**
     ------------------------------------------------------------------------------------
     load atomic  acquire      - singlethread - global   1. buffer/global/ds/flat_load
                               - wavefront    - local
                                              - generic
     load atomic  acquire      - workgroup    - global   1. buffer/global_load sc0=1
                                                         2. s_waitcnt vmcnt(0)

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before the
                                                             following buffer_inv.

                                                         3. buffer_inv sc0=1

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     load atomic  acquire      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_load
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local load
                                                             atomic value being
                                                             acquired.

     load atomic  acquire      - workgroup    - generic  1. flat_load  sc0=1
                                                         2. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit lgkmcnt(0).
                                                           - Must happen before
                                                             the following
                                                             buffer_inv and any
                                                             following global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local load
                                                             atomic value being
                                                             acquired.

                                                         3. buffer_inv sc0=1

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     load atomic  acquire      - agent        - global   1. buffer/global_load
                                                            sc1=1
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the load
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_inv sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale global data.

     load atomic  acquire      - system       - global   1. buffer/global/flat_load
                                                            sc0=1 sc1=1
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the load
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_inv sc0=1 sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale MTYPE NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale due to the
                                                             memory probes.

     load atomic  acquire      - agent        - generic  1. flat_load sc1=1
                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the flat_load
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_inv sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     load atomic  acquire      - system       - generic  1. flat_load sc0=1 sc1=1
                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             the following
                                                             buffer_inv.
                                                           - Ensures the flat_load
                                                             has completed
                                                             before invalidating
                                                             the caches.

                                                         3. buffer_inv sc0=1 sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale MTYPE NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale due to the
                                                             memory probes.

     atomicrmw    acquire      - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
     atomicrmw    acquire      - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                                                         be used.*

                                                         1. ds_atomic
     atomicrmw    acquire      - workgroup    - global   1. buffer/global_atomic
                                                         2. s_waitcnt vmcnt(0)

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before the
                                                             following buffer_inv.
                                                           - Ensures the atomicrmw
                                                             has completed
                                                             before invalidating
                                                             the cache.

                                                         3. buffer_inv sc0=1

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_atomic
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local
                                                             atomicrmw value
                                                             being acquired.

     atomicrmw    acquire      - workgroup    - generic  1. flat_atomic
                                                         2. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit lgkmcnt(0).
                                                           - Must happen before
                                                             the following
                                                             buffer_inv and
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local
                                                             atomicrmw value
                                                             being acquired.

                                                         3. buffer_inv sc0=1

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acquire      - agent        - global   1. buffer/global_atomic
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         3. buffer_inv sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - system       - global   1. buffer/global_atomic
                                                            sc1=1
                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         3. buffer_inv sc0=1 sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale MTYPE NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale due to the
                                                             memory probes.

     atomicrmw    acquire      - agent        - generic  1. flat_atomic
                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         3. buffer_inv sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - system       - generic  1. flat_atomic sc1=1
                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         3. buffer_inv sc0=1 sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale MTYPE NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale due to the
                                                             memory probes.

     fence        acquire      - singlethread *none*     *none*
                               - wavefront
     fence        acquire      - workgroup    *none*     1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/
                                                             atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following
                                                             buffer_inv and
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the
                                                             value read by the
                                                             fence-paired-atomic.

                                                         3. buffer_inv sc0=1

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     fence        acquire      - agent        *none*     1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following
                                                             buffer_inv.
                                                           - Ensures that the
                                                             fence-paired atomic
                                                             has completed
                                                             before invalidating
                                                             the
                                                             cache. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             fence-paired-atomic.

                                                         2. buffer_inv sc1=1

                                                           - Must happen before any
                                                             following global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     fence        acquire      - system       *none*     1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following
                                                             buffer_inv.
                                                           - Ensures that the
                                                             fence-paired atomic
                                                             has completed
                                                             before invalidating
                                                             the
                                                             cache. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             fence-paired-atomic.

                                                         2. buffer_inv sc0=1 sc1=1

                                                           - Must happen before any
                                                             following global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     **Release Atomic**
     ------------------------------------------------------------------------------------
     store atomic release      - singlethread - global   1. buffer/global/flat_store
                               - wavefront    - generic
     store atomic release      - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                                                         be used.*

                                                         1. ds_store
     store atomic release      - workgroup    - global   1. s_waitcnt lgkm/vmcnt(0)
                                              - generic
                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit lgkmcnt(0).
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             store.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         2. buffer/global/flat_store sc0=1
     store atomic release      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_store
     store atomic release      - agent        - global   1. buffer_wbl2 sc1=1
                                              - generic
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at agent scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             store.
                                                           - Ensures that all
                                                             memory operations
                                                             to memory have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         3. buffer/global/flat_store sc1=1
     store atomic release      - system       - global   1. buffer_wbl2 sc0=1 sc1=1
                                              - generic
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after any
                                                             preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after any
                                                             preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             store.
                                                           - Ensures that all
                                                             memory operations
                                                             to memory and the L2
                                                             writeback have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         3. buffer/global/flat_store
                                                            sc0=1 sc1=1
     atomicrmw    release      - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
     atomicrmw    release      - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                                                         be used.*

                                                         1. ds_atomic
     atomicrmw    release      - workgroup    - global   1. s_waitcnt lgkm/vmcnt(0)
                                              - generic
                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. buffer/global/flat_atomic sc0=1
     atomicrmw    release      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_atomic
     atomicrmw    release      - agent        - global   1. buffer_wbl2 sc1=1
                                              - generic
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at agent scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global and local
                                                             have completed
                                                             before performing
                                                             the atomicrmw that
                                                             is being released.

                                                         3. buffer/global/flat_atomic sc1=1
     atomicrmw    release      - system       - global   1. buffer_wbl2 sc0=1 sc1=1
                                              - generic
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to memory and the L2
                                                             writeback have
                                                             completed before
                                                             performing the
                                                             store that is being
                                                             released.

                                                         3. buffer/global/flat_atomic
                                                            sc0=1 sc1=1
     fence        release      - singlethread *none*     *none*
                               - wavefront
     fence        release      - workgroup    *none*     1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             following
                                                             fence-paired-atomic.

     fence        release      - agent        *none*     1. buffer_wbl2 sc1=1

                                                           - If OpenCL and
                                                             address space is
                                                             local, omit.
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at agent scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             following
                                                             fence-paired-atomic.

     fence        release      - system       *none*     1. buffer_wbl2 sc0=1 sc1=1

                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             following
                                                             fence-paired-atomic.

     **Acquire-Release Atomic**
     ------------------------------------------------------------------------------------
     atomicrmw    acq_rel      - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
     atomicrmw    acq_rel      - singlethread - local    *If TgSplit execution mode,
                               - wavefront               local address space cannot
                                                         be used.*

                                                         1. ds_atomic
     atomicrmw    acq_rel      - workgroup    - global   1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. buffer/global_atomic
                                                         3. s_waitcnt vmcnt(0)

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Must happen before
                                                             the following
                                                             buffer_inv.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the
                                                             atomicrmw value
                                                             being acquired.

                                                         4. buffer_inv sc0=1

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acq_rel      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         1. ds_atomic
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local load
                                                             atomic value being
                                                             acquired.

     atomicrmw    acq_rel      - workgroup    - generic  1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         2. flat_atomic
                                                         3. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If not TgSplit execution
                                                             mode, omit vmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             the following
                                                             buffer_inv and
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local load
                                                             atomic value being
                                                             acquired.

                                                         3. buffer_inv sc0=1

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acq_rel      - agent        - global   1. buffer_wbl2 sc1=1

                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at agent scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         3. buffer/global_atomic
                                                         4. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         5. buffer_inv sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acq_rel      - system       - global   1. buffer_wbl2 sc0=1 sc1=1

                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global and L2 writeback
                                                             have completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         3. buffer/global_atomic
                                                            sc1=1
                                                         4. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         5. buffer_inv sc0=1 sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             MTYPE NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale due to the
                                                             memory probes.

     atomicrmw    acq_rel      - agent        - generic  1. buffer_wbl2 sc1=1

                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at agent scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global have
                                                             completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         3. flat_atomic
                                                         4. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             cache.

                                                         5. buffer_inv sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acq_rel      - system       - generic  1. buffer_wbl2 sc0=1 sc1=1

                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             to global and L2 writeback
                                                             have completed before
                                                             performing the
                                                             atomicrmw that is
                                                             being released.

                                                         3. flat_atomic sc1=1
                                                         4. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         5. buffer_inv sc0=1 sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             MTYPE NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale due to the
                                                             memory probes.

     fence        acq_rel      - singlethread *none*     *none*
                               - wavefront
     fence        acq_rel      - workgroup    *none*     1. s_waitcnt lgkm/vmcnt(0)

                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0).
                                                           - However,
                                                             since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/
                                                             load atomic/store atomic/
                                                             atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that all
                                                             memory operations
                                                             have
                                                             completed before
                                                             performing any
                                                             following global
                                                             memory operations.
                                                           - Ensures that the
                                                             preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             acquire-fence-paired-atomic)
                                                             has completed
                                                             before following
                                                             global memory
                                                             operations. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.
                                                           - Ensures that all
                                                             previous memory
                                                             operations have
                                                             completed before a
                                                             following
                                                             local/generic store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             release-fence-paired-atomic).
                                                             This satisfies the
                                                             requirements of
                                                             release.
                                                           - Must happen before
                                                             the following
                                                             buffer_inv.
                                                           - Ensures that the
                                                             acquire-fence-paired
                                                             atomic has completed
                                                             before invalidating
                                                             the
                                                             cache. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             acquire-fence-paired-atomic.

                                                         3. buffer_inv sc0=1

                                                           - If not TgSplit execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     fence        acq_rel      - agent        *none*     1. buffer_wbl2 sc1=1

                                                           - If OpenCL and
                                                             address space is
                                                             local, omit.
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at agent scope.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             buffer_inv.
                                                           - Ensures that the
                                                             preceding
                                                             global/local/generic
                                                             load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             acquire-fence-paired-atomic)
                                                             has completed
                                                             before invalidating
                                                             the cache. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.
                                                           - Ensures that all
                                                             previous memory
                                                             operations have
                                                             completed before a
                                                             following
                                                             global/local/generic
                                                             store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             release-fence-paired-atomic).
                                                             This satisfies the
                                                             requirements of
                                                             release.

                                                         3. buffer_inv sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.

     fence        acq_rel      - system       *none*     1. buffer_wbl2 sc0=1 sc1=1

                                                           - If OpenCL and
                                                             address space is
                                                             local, omit.
                                                           - Must happen before
                                                             following s_waitcnt.
                                                           - Performs L2 writeback to
                                                             ensure previous
                                                             global/generic
                                                             store/atomicrmw are
                                                             visible at system scope.

                                                         1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0) and
                                                             s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic
                                                             load/store/load
                                                             atomic/store
                                                             atomic/atomicrmw.
                                                           - Must happen before
                                                             the following
                                                             buffer_inv.
                                                           - Ensures that the
                                                             preceding
                                                             global/local/generic
                                                             load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             acquire-fence-paired-atomic)
                                                             has completed
                                                             before invalidating
                                                             the cache. This
                                                             satisfies the
                                                             requirements of
                                                             acquire.
                                                           - Ensures that all
                                                             previous memory
                                                             operations have
                                                             completed before a
                                                             following
                                                             global/local/generic
                                                             store
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             release-fence-paired-atomic).
                                                             This satisfies the
                                                             requirements of
                                                             release.

                                                         2. buffer_inv sc0=1 sc1=1

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             MTYPE NC global data.
                                                             MTYPE RW and CC memory will
                                                             never be stale due to the
                                                             memory probes.

     **Sequential Consistent Atomic**
     ------------------------------------------------------------------------------------
     load atomic  seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    load atomic acquire,
                                              - generic  except must generate
                                                         all instructions even
                                                         for OpenCL.*
     load atomic  seq_cst      - workgroup    - global   1. s_waitcnt lgkm/vmcnt(0)
                                              - generic
                                                           - Use lgkmcnt(0) if not
                                                             TgSplit execution mode
                                                             and vmcnt(0) if TgSplit
                                                             execution mode.
                                                           - s_waitcnt lgkmcnt(0) must
                                                             happen after
                                                             preceding
                                                             local/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             lgkmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             preceding
                                                             global/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             vmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - Ensures any
                                                             preceding
                                                             sequential
                                                             consistent global/local
                                                             memory instructions
                                                             have completed
                                                             before executing
                                                             this sequentially
                                                             consistent
                                                             instruction. This
                                                             prevents reordering
                                                             a seq_cst store
                                                             followed by a
                                                             seq_cst load. (Note
                                                             that seq_cst is
                                                             stronger than
                                                             acquire/release as
                                                             the reordering of
                                                             load acquire
                                                             followed by a store
                                                             release is
                                                             prevented by the
                                                             s_waitcnt of
                                                             the release, but
                                                             there is nothing
                                                             preventing a store
                                                             release followed by
                                                             load acquire from
                                                             completing out of
                                                             order. The s_waitcnt
                                                             could be placed after
                                                             seq_store or before
                                                             the seq_load. We
                                                             choose the load to
                                                             make the s_waitcnt be
                                                             as late as possible
                                                             so that the store
                                                             may have already
                                                             completed.)

                                                         2. *Following
                                                            instructions same as
                                                            corresponding load
                                                            atomic acquire,
                                                            except must generate
                                                            all instructions even
                                                            for OpenCL.*
     load atomic  seq_cst      - workgroup    - local    *If TgSplit execution mode,
                                                         local address space cannot
                                                         be used.*

                                                         *Same as corresponding
                                                         load atomic acquire,
                                                         except must generate
                                                         all instructions even
                                                         for OpenCL.*

     load atomic  seq_cst      - agent        - global   1. s_waitcnt lgkmcnt(0) &
                               - system       - generic     vmcnt(0)

                                                           - If TgSplit execution mode,
                                                             omit lgkmcnt(0).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0)
                                                             and s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             preceding
                                                             global/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             lgkmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             preceding
                                                             global/generic load
                                                             atomic/store
                                                             atomic/atomicrmw
                                                             with memory
                                                             ordering of seq_cst
                                                             and with equal or
                                                             wider sync scope.
                                                             (Note that seq_cst
                                                             fences have their
                                                             own s_waitcnt
                                                             vmcnt(0) and so do
                                                             not need to be
                                                             considered.)
                                                           - Ensures any
                                                             preceding
                                                             sequential
                                                             consistent global
                                                             memory instructions
                                                             have completed
                                                             before executing
                                                             this sequentially
                                                             consistent
                                                             instruction. This
                                                             prevents reordering
                                                             a seq_cst store
                                                             followed by a
                                                             seq_cst load. (Note
                                                             that seq_cst is
                                                             stronger than
                                                             acquire/release as
                                                             the reordering of
                                                             load acquire
                                                             followed by a store
                                                             release is
                                                             prevented by the
                                                             s_waitcnt of
                                                             the release, but
                                                             there is nothing
                                                             preventing a store
                                                             release followed by
                                                             load acquire from
                                                             completing out of
                                                             order. The s_waitcnt
                                                             could be placed after
                                                             seq_store or before
                                                             the seq_load. We
                                                             choose the load to
                                                             make the s_waitcnt be
                                                             as late as possible
                                                             so that the store
                                                             may have already
                                                             completed.)

                                                         2. *Following
                                                            instructions same as
                                                            corresponding load
                                                            atomic acquire,
                                                            except must generate
                                                            all instructions even
                                                            for OpenCL.*
     store atomic seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    store atomic release,
                               - workgroup    - generic  except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     atomicrmw    seq_cst      - singlethread - global   *Same as corresponding
                               - wavefront    - local    atomicrmw acq_rel,
                               - workgroup    - generic  except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     fence        seq_cst      - singlethread *none*     *Same as corresponding
                               - wavefront               fence acq_rel,
                               - workgroup               except must generate
                               - agent                   all instructions even
                               - system                  for OpenCL.*
     ============ ============ ============== ========== ================================

.. _amdgpu-amdhsa-memory-model-gfx10-gfx11:

Memory Model GFX10-GFX11
++++++++++++++++++++++++

For GFX10-GFX11:

* Each agent has multiple shader arrays (SA).
* Each SA has multiple work-group processors (WGP).
* Each WGP has multiple compute units (CU).
* Each CU has multiple SIMDs that execute wavefronts.
* The wavefronts for a single work-group are executed in the same
  WGP. In CU wavefront execution mode the wavefronts may be executed by
  different SIMDs in the same CU. In WGP wavefront execution mode the
  wavefronts may be executed by different SIMDs in different CUs in the same
  WGP.
* Each WGP has a single LDS memory shared by the wavefronts of the work-groups
  executing on it.
* All LDS operations of a WGP are performed as wavefront wide operations in a
  global order and involve no caching. Completion is reported to a wavefront in
  execution order.
* The LDS memory has multiple request queues shared by the SIMDs of a
  WGP. Therefore, the LDS operations performed by different wavefronts of a
  work-group can be reordered relative to each other, which can result in
  reordering the visibility of vector memory operations with respect to LDS
  operations of other wavefronts in the same work-group. A ``s_waitcnt
  lgkmcnt(0)`` is required to ensure synchronization between LDS operations and
  vector memory operations between wavefronts of a work-group, but not between
  operations performed by the same wavefront.
* The vector memory operations are performed as wavefront wide operations.
  Completion of load/store/sample operations are reported to a wavefront in
  execution order of other load/store/sample operations performed by that
  wavefront.
* The vector memory operations access a vector L0 cache. There is a single L0
  cache per CU. Each SIMD of a CU accesses the same L0 cache. Therefore, no
  special action is required for coherence between the lanes of a single
  wavefront. However, a ``buffer_gl0_inv`` is required for coherence between
  wavefronts executing in the same work-group as they may be executing on SIMDs
  of different CUs that access different L0s. A ``buffer_gl0_inv`` is also
  required for coherence between wavefronts executing in different work-groups
  as they may be executing on different WGPs.
* The scalar memory operations access a scalar L0 cache shared by all wavefronts
  on a WGP. The scalar and vector L0 caches are not coherent. However, scalar
  operations are used in a restricted way so do not impact the memory model. See
  :ref:`amdgpu-amdhsa-memory-spaces`.
* The vector and scalar memory L0 caches use an L1 cache shared by all WGPs on
  the same SA. Therefore, no special action is required for coherence between
  the wavefronts of a single work-group. However, a ``buffer_gl1_inv`` is
  required for coherence between wavefronts executing in different work-groups
  as they may be executing on different SAs that access different L1s.
* The L1 caches have independent quadrants to service disjoint ranges of virtual
  addresses.
* Each L0 cache has a separate request queue per L1 quadrant. Therefore, the
  vector and scalar memory operations performed by different wavefronts, whether
  executing in the same or different work-groups (which may be executing on
  different CUs accessing different L0s), can be reordered relative to each
  other. A ``s_waitcnt vmcnt(0) & vscnt(0)`` is required to ensure
  synchronization between vector memory operations of different wavefronts. It
  ensures a previous vector memory operation has completed before executing a
  subsequent vector memory or LDS operation and so can be used to meet the
  requirements of acquire, release and sequential consistency.
* The L1 caches use an L2 cache shared by all SAs on the same agent.
* The L2 cache has independent channels to service disjoint ranges of virtual
  addresses.
* Each L1 quadrant of a single SA accesses a different L2 channel. Each L1
  quadrant has a separate request queue per L2 channel. Therefore, the vector
  and scalar memory operations performed by wavefronts executing in different
  work-groups (which may be executing on different SAs) of an agent can be
  reordered relative to each other. A ``s_waitcnt vmcnt(0) & vscnt(0)`` is
  required to ensure synchronization between vector memory operations of
  different SAs. It ensures a previous vector memory operation has completed
  before executing a subsequent vector memory and so can be used to meet the
  requirements of acquire, release and sequential consistency.
* The L2 cache can be kept coherent with other agents on some targets, or ranges
  of virtual addresses can be set up to bypass it to ensure system coherence.
* On GFX10.3 and GFX11 a memory attached last level (MALL) cache exists for GPU memory.
  The MALL cache is fully coherent with GPU memory and has no impact on system
  coherence. All agents (GPU and CPU) access GPU memory through the MALL cache.

Scalar memory operations are only used to access memory that is proven to not
change during the execution of the kernel dispatch. This includes constant
address space and global address space for program scope ``const`` variables.
Therefore, the kernel machine code does not have to maintain the scalar cache to
ensure it is coherent with the vector caches. The scalar and vector caches are
invalidated between kernel dispatches by CP since constant address space data
may change between kernel dispatch executions. See
:ref:`amdgpu-amdhsa-memory-spaces`.

The one exception is if scalar writes are used to spill SGPR registers. In this
case the AMDGPU backend ensures the memory location used to spill is never
accessed by vector memory operations at the same time. If scalar writes are used
then a ``s_dcache_wb`` is inserted before the ``s_endpgm`` and before a function
return since the locations may be used for vector memory instructions by a
future wavefront that uses the same scratch area, or a function call that
creates a frame at the same address, respectively. There is no need for a
``s_dcache_inv`` as all scalar writes are write-before-read in the same thread.

For kernarg backing memory:

* CP invalidates the L0 and L1 caches at the start of each kernel dispatch.
* On dGPU the kernarg backing memory is accessed as MTYPE UC (uncached) to avoid
  needing to invalidate the L2 cache.
* On APU the kernarg backing memory is accessed as MTYPE CC (cache coherent) and
  so the L2 cache will be coherent with the CPU and other agents.

Scratch backing memory (which is used for the private address space) is accessed
with MTYPE NC (non-coherent). Since the private address space is only accessed
by a single thread, and is always write-before-read, there is never a need to
invalidate these entries from the L0 or L1 caches.

Wavefronts are executed in native mode with in-order reporting of loads and
sample instructions. In this mode vmcnt reports completion of load, atomic with
return and sample instructions in order, and the vscnt reports the completion of
store and atomic without return in order. See ``MEM_ORDERED`` field in
:ref:`amdgpu-amdhsa-compute_pgm_rsrc1-gfx6-gfx11-table`.

Wavefronts can be executed in WGP or CU wavefront execution mode:

* In WGP wavefront execution mode the wavefronts of a work-group are executed
  on the SIMDs of both CUs of the WGP. Therefore, explicit management of the per
  CU L0 caches is required for work-group synchronization. Also accesses to L1
  at work-group scope need to be explicitly ordered as the accesses from
  different CUs are not ordered.
* In CU wavefront execution mode the wavefronts of a work-group are executed on
  the SIMDs of a single CU of the WGP. Therefore, all global memory access by
  the work-group access the same L0 which in turn ensures L1 accesses are
  ordered and so do not require explicit management of the caches for
  work-group synchronization.

See ``WGP_MODE`` field in
:ref:`amdgpu-amdhsa-compute_pgm_rsrc1-gfx6-gfx11-table` and
:ref:`amdgpu-target-features`.

The code sequences used to implement the memory model for GFX10-GFX11 are defined in
table :ref:`amdgpu-amdhsa-memory-model-code-sequences-gfx10-gfx11-table`.

  .. table:: AMDHSA Memory Model Code Sequences GFX10-GFX11
     :name: amdgpu-amdhsa-memory-model-code-sequences-gfx10-gfx11-table

     ============ ============ ============== ========== ================================
     LLVM Instr   LLVM Memory  LLVM Memory    AMDGPU     AMDGPU Machine Code
                  Ordering     Sync Scope     Address    GFX10-GFX11
                                              Space
     ============ ============ ============== ========== ================================
     **Non-Atomic**
     ------------------------------------------------------------------------------------
     load         *none*       *none*         - global   - !volatile & !nontemporal
                                              - generic
                                              - private    1. buffer/global/flat_load
                                              - constant
                                                         - !volatile & nontemporal

                                                           1. buffer/global/flat_load
                                                              slc=1 dlc=1

                                                            - If GFX10, omit dlc=1.

                                                         - volatile

                                                           1. buffer/global/flat_load
                                                              glc=1 dlc=1

                                                           2. s_waitcnt vmcnt(0)

                                                            - Must happen before
                                                              any following volatile
                                                              global/generic
                                                              load/store.
                                                            - Ensures that
                                                              volatile
                                                              operations to
                                                              different
                                                              addresses will not
                                                              be reordered by
                                                              hardware.

     load         *none*       *none*         - local    1. ds_load
     store        *none*       *none*         - global   - !volatile & !nontemporal
                                              - generic
                                              - private    1. buffer/global/flat_store
                                              - constant
                                                         - !volatile & nontemporal

                                                           1. buffer/global/flat_store
                                                              glc=1 slc=1 dlc=1

                                                            - If GFX10, omit dlc=1.

                                                         - volatile

                                                           1. buffer/global/flat_store
                                                              dlc=1

                                                            - If GFX10, omit dlc=1.

                                                           2. s_waitcnt vscnt(0)

                                                            - Must happen before
                                                              any following volatile
                                                              global/generic
                                                              load/store.
                                                            - Ensures that
                                                              volatile
                                                              operations to
                                                              different
                                                              addresses will not
                                                              be reordered by
                                                              hardware.

     store        *none*       *none*         - local    1. ds_store
     **Unordered Atomic**
     ------------------------------------------------------------------------------------
     load atomic  unordered    *any*          *any*      *Same as non-atomic*.
     store atomic unordered    *any*          *any*      *Same as non-atomic*.
     atomicrmw    unordered    *any*          *any*      *Same as monotonic atomic*.
     **Monotonic Atomic**
     ------------------------------------------------------------------------------------
     load atomic  monotonic    - singlethread - global   1. buffer/global/flat_load
                               - wavefront    - generic
     load atomic  monotonic    - workgroup    - global   1. buffer/global/flat_load
                                              - generic     glc=1

                                                           - If CU wavefront execution
                                                             mode, omit glc=1.

     load atomic  monotonic    - singlethread - local    1. ds_load
                               - wavefront
                               - workgroup
     load atomic  monotonic    - agent        - global   1. buffer/global/flat_load
                               - system       - generic     glc=1 dlc=1

                                                           - If GFX11, omit dlc=1.

     store atomic monotonic    - singlethread - global   1. buffer/global/flat_store
                               - wavefront    - generic
                               - workgroup
                               - agent
                               - system
     store atomic monotonic    - singlethread - local    1. ds_store
                               - wavefront
                               - workgroup
     atomicrmw    monotonic    - singlethread - global   1. buffer/global/flat_atomic
                               - wavefront    - generic
                               - workgroup
                               - agent
                               - system
     atomicrmw    monotonic    - singlethread - local    1. ds_atomic
                               - wavefront
                               - workgroup
     **Acquire Atomic**
     ------------------------------------------------------------------------------------
     load atomic  acquire      - singlethread - global   1. buffer/global/ds/flat_load
                               - wavefront    - local
                                              - generic
     load atomic  acquire      - workgroup    - global   1. buffer/global_load glc=1

                                                           - If CU wavefront execution
                                                             mode, omit glc=1.

                                                         2. s_waitcnt vmcnt(0)

                                                           - If CU wavefront execution
                                                             mode, omit.
                                                           - Must happen before
                                                             the following buffer_gl0_inv
                                                             and before any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.

                                                         3. buffer_gl0_inv

                                                           - If CU wavefront execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     load atomic  acquire      - workgroup    - local    1. ds_load
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             the following buffer_gl0_inv
                                                             and before any following
                                                             global/generic load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local load
                                                             atomic value being
                                                             acquired.

                                                         3. buffer_gl0_inv

                                                           - If CU wavefront execution
                                                             mode, omit.
                                                           - If OpenCL, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     load atomic  acquire      - workgroup    - generic  1. flat_load glc=1

                                                           - If CU wavefront execution
                                                             mode, omit glc=1.

                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0)

                                                           - If CU wavefront execution
                                                             mode, omit vmcnt(0).
                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             the following
                                                             buffer_gl0_inv and any
                                                             following global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local load
                                                             atomic value being
                                                             acquired.

                                                         3. buffer_gl0_inv

                                                           - If CU wavefront execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     load atomic  acquire      - agent        - global   1. buffer/global_load
                               - system                     glc=1 dlc=1

                                                           - If GFX11, omit dlc=1.

                                                         2. s_waitcnt vmcnt(0)

                                                           - Must happen before
                                                             following
                                                             buffer_gl*_inv.
                                                           - Ensures the load
                                                             has completed
                                                             before invalidating
                                                             the caches.

                                                         3. buffer_gl0_inv;
                                                            buffer_gl1_inv

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale global data.

     load atomic  acquire      - agent        - generic  1. flat_load glc=1 dlc=1
                               - system
                                                           - If GFX11, omit dlc=1.

                                                         2. s_waitcnt vmcnt(0) &
                                                            lgkmcnt(0)

                                                           - If OpenCL omit
                                                             lgkmcnt(0).
                                                           - Must happen before
                                                             following
                                                             buffer_gl*_invl.
                                                           - Ensures the flat_load
                                                             has completed
                                                             before invalidating
                                                             the caches.

                                                         3. buffer_gl0_inv;
                                                            buffer_gl1_inv

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - singlethread - global   1. buffer/global/ds/flat_atomic
                               - wavefront    - local
                                              - generic
     atomicrmw    acquire      - workgroup    - global   1. buffer/global_atomic
                                                         2. s_waitcnt vm/vscnt(0)

                                                           - If CU wavefront execution
                                                             mode, omit.
                                                           - Use vmcnt(0) if atomic with
                                                             return and vscnt(0) if
                                                             atomic with no-return.
                                                           - Must happen before
                                                             the following buffer_gl0_inv
                                                             and before any following
                                                             global/generic
                                                             load/load
                                                             atomic/store/store
                                                             atomic/atomicrmw.

                                                         3. buffer_gl0_inv

                                                           - If CU wavefront execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acquire      - workgroup    - local    1. ds_atomic
                                                         2. s_waitcnt lgkmcnt(0)

                                                           - If OpenCL, omit.
                                                           - Must happen before
                                                             the following
                                                             buffer_gl0_inv.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than the local
                                                             atomicrmw value
                                                             being acquired.

                                                         3. buffer_gl0_inv

                                                           - If OpenCL omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acquire      - workgroup    - generic  1. flat_atomic
                                                         2. s_waitcnt lgkmcnt(0) &
                                                            vm/vscnt(0)

                                                           - If CU wavefront execution
                                                             mode, omit vm/vscnt(0).
                                                           - If OpenCL, omit lgkmcnt(0).
                                                           - Use vmcnt(0) if atomic with
                                                             return and vscnt(0) if
                                                             atomic with no-return.
                                                           - Must happen before
                                                             the following
                                                             buffer_gl0_inv.
                                                           - Ensures any
                                                             following global
                                                             data read is no
                                                             older than a local
                                                             atomicrmw value
                                                             being acquired.

                                                         3. buffer_gl0_inv

                                                           - If CU wavefront execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     atomicrmw    acquire      - agent        - global   1. buffer/global_atomic
                               - system                  2. s_waitcnt vm/vscnt(0)

                                                           - Use vmcnt(0) if atomic with
                                                             return and vscnt(0) if
                                                             atomic with no-return.
                                                           - Must happen before
                                                             following
                                                             buffer_gl*_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         3. buffer_gl0_inv;
                                                            buffer_gl1_inv

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     atomicrmw    acquire      - agent        - generic  1. flat_atomic
                               - system                  2. s_waitcnt vm/vscnt(0) &
                                                            lgkmcnt(0)

                                                           - If OpenCL, omit
                                                             lgkmcnt(0).
                                                           - Use vmcnt(0) if atomic with
                                                             return and vscnt(0) if
                                                             atomic with no-return.
                                                           - Must happen before
                                                             following
                                                             buffer_gl*_inv.
                                                           - Ensures the
                                                             atomicrmw has
                                                             completed before
                                                             invalidating the
                                                             caches.

                                                         3. buffer_gl0_inv;
                                                            buffer_gl1_inv

                                                           - Must happen before
                                                             any following
                                                             global/generic
                                                             load/load
                                                             atomic/atomicrmw.
                                                           - Ensures that
                                                             following loads
                                                             will not see stale
                                                             global data.

     fence        acquire      - singlethread *none*     *none*
                               - wavefront
     fence        acquire      - workgroup    *none*     1. s_waitcnt lgkmcnt(0) &
                                                            vmcnt(0) & vscnt(0)

                                                           - If CU wavefront execution
                                                             mode, omit vmcnt(0) and
                                                             vscnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0) and vscnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate. If
                                                             fence had an
                                                             address space then
                                                             set to address
                                                             space of OpenCL
                                                             fence flag, or to
                                                             generic if both
                                                             local and global
                                                             flags are
                                                             specified.
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0), s_waitcnt
                                                             vscnt(0) and s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/
                                                             atomicrmw-with-return-value
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt vscnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             atomicrmw-no-return-value
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following
                                                             buffer_gl0_inv.
                                                           - Ensures that the
                                                             fence-paired atomic
                                                             has completed
                                                             before invalidating
                                                             the
                                                             cache. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             fence-paired-atomic.

                                                         3. buffer_gl0_inv

                                                           - If CU wavefront execution
                                                             mode, omit.
                                                           - Ensures that
                                                             following
                                                             loads will not see
                                                             stale data.

     fence        acquire      - agent        *none*     1. s_waitcnt lgkmcnt(0) &
                               - system                     vmcnt(0) & vscnt(0)

                                                           - If OpenCL and
                                                             address space is
                                                             not generic, omit
                                                             lgkmcnt(0).
                                                           - If OpenCL and
                                                             address space is
                                                             local, omit
                                                             vmcnt(0) and vscnt(0).
                                                           - However, since LLVM
                                                             currently has no
                                                             address space on
                                                             the fence need to
                                                             conservatively
                                                             always generate
                                                             (see comment for
                                                             previous fence).
                                                           - Could be split into
                                                             separate s_waitcnt
                                                             vmcnt(0), s_waitcnt
                                                             vscnt(0) and s_waitcnt
                                                             lgkmcnt(0) to allow
                                                             them to be
                                                             independently moved
                                                             according to the
                                                             following rules.
                                                           - s_waitcnt vmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic load
                                                             atomic/
                                                             atomicrmw-with-return-value
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt vscnt(0)
                                                             must happen after
                                                             any preceding
                                                             global/generic
                                                             atomicrmw-no-return-value
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - s_waitcnt lgkmcnt(0)
                                                             must happen after
                                                             any preceding
                                                             local/generic load
                                                             atomic/atomicrmw
                                                             with an equal or
                                                             wider sync scope
                                                             and memory ordering
                                                             stronger than
                                                             unordered (this is
                                                             termed the
                                                             fence-paired-atomic).
                                                           - Must happen before
                                                             the following
                                                             buffer_gl*_inv.
                                                           - Ensures that the
                                                             fence-paired atomic
                                                             has completed
                                                             before invalidating
                                                             the
                                                             caches. Therefore
                                                             any following
                                                             locations read must
                                                             be no older than
                                                             the value read by
                                                             the
                                                             fence-paired-atomic.

                                                         2. buffer_gl0_inv;
                                                            buffer_gl1_inv

                                                           - Must happen before any
                                                             following global/generic
                                                             load/load
                                                             atomic/store