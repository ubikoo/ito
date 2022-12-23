CFLAGS   += -I$(ROOTDIR)

# -----------------------------------------------------------------------------
# core module
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/ito/core/_*.cpp), \
                         $(wildcard $(ROOTDIR)/ito/core/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/ito/core/_*.c), \
                         $(wildcard $(ROOTDIR)/ito/core/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/ito/core/*.hpp) \
			$(wildcard $(ROOTDIR)/ito/core/*.h)

# -----------------------------------------------------------------------------
# math module
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/ito/math/_*.cpp), \
                         $(wildcard $(ROOTDIR)/ito/math/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/ito/math/_*.c), \
                         $(wildcard $(ROOTDIR)/ito/math/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/ito/math/*.hpp) \
			$(wildcard $(ROOTDIR)/ito/math/*.h)

# -----------------------------------------------------------------------------
# opencl module
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/ito/opencl/_*.cpp), \
                         $(wildcard $(ROOTDIR)/ito/opencl/*.cpp)) \
            $(filter-out $(wildcard $(ROOTDIR)/ito/opencl/_*.c), \
                         $(wildcard $(ROOTDIR)/ito/opencl/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/ito/opencl/*.hpp) \
            $(wildcard $(ROOTDIR)/ito/opencl/*.h)

# -----------------------------------------------------------------------------
# opengl module
SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/ito/opengl/_*.cpp), \
                         $(wildcard $(ROOTDIR)/ito/opengl/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/ito/opengl/_*.c), \
                         $(wildcard $(ROOTDIR)/ito/opengl/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/ito/opengl/*.hpp) \
			$(wildcard $(ROOTDIR)/ito/opengl/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/ito/opengl/glsl/_*.cpp), \
                         $(wildcard $(ROOTDIR)/ito/opengl/glsl/*.cpp)) \
            $(filter-out $(wildcard $(ROOTDIR)/ito/opengl/glsl/_*.c), \
                         $(wildcard $(ROOTDIR)/ito/opengl/glsl/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/ito/opengl/glsl/*.hpp) \
            $(wildcard $(ROOTDIR)/ito/opengl/glsl/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/ito/opengl/graphics/_*.cpp), \
                         $(wildcard $(ROOTDIR)/ito/opengl/graphics/*.cpp)) \
            $(filter-out $(wildcard $(ROOTDIR)/ito/opengl/graphics/_*.c), \
                         $(wildcard $(ROOTDIR)/ito/opengl/graphics/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/ito/opengl/graphics/*.hpp) \
            $(wildcard $(ROOTDIR)/ito/opengl/graphics/*.h)
