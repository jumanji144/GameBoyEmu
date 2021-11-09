#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "debugger.h"
#include "imgui_memory_editor.h"
#include "include.h"
#include "memory.h"
#include "rom.h"
#include "emulator.h"
#include "imgui_internal.h"

using namespace ImGui;

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void drop_callback(GLFWwindow *window, int count, const char **paths)
{
    const char* romFile = paths[0];

    if (strstr(romFile, ".gb") != NULL)
    {
        printf("Loading %s\n", romFile);
        Rom* rom = loadRom(romFile);

        if (rom != NULL)
        {
            printf("Loaded %s\n", romFile);
            load_rom(rom);
            init(rom);
            attach_emulator(); // load memory again
            free(rom);
        }
    }
}

ImVec4 clear_color = ImVec4(0, 0, 0, 1);
GLFWwindow *window;
static void *g_mem_ptr;
static u8 *g_rom_ptr;

void init_debugview()
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window with graphics context

    window = glfwCreateWindow(1280, 720, "System Mointor", NULL, NULL);
    if (window == NULL)
        return;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void attach_emulator()
{
    g_mem_ptr = request_memory();
    g_rom_ptr = (u8*) g_mem_ptr;
}

int should_close_debugview()
{
    return glfwWindowShouldClose(window);
}

static void sprint_hex(char* buffer, u8 val) {
  const char hex_digits[] = "0123456789abcdef";
  buffer[0] = hex_digits[(val >> 4) & 0xf];
  buffer[1] = hex_digits[val & 0xf];
}

static int disassemble_instr(u8 data[3], char* buffer, size_t size) {
  char temp[80];
  u8 opcode = data[0];
  u8 num_bytes = s_opcode_bytes[opcode];
  switch (num_bytes) {
    case 1: {
      const char* mnemonic = s_opcode_mnemonic[opcode];
      if (!mnemonic) {
        mnemonic = "*INVALID*";
      }
      strncpy(temp, mnemonic, sizeof(temp) - 1);
      break;
    }
    case 2:
      if (opcode == 0xcb) {
        strncpy(temp, s_cb_opcode_mnemonic[data[1]], sizeof(temp) - 1);
      } else {
        snprintf(temp, sizeof(temp), s_opcode_mnemonic[opcode], data[1]);
      }
      break;
    case 3:
      snprintf(temp, sizeof(temp), s_opcode_mnemonic[opcode],
               (data[2] << 8) | data[1]);
      break;
    default: assert(!"invalid opcode byte length.\n"); break;
  }

  char hex[][3] = {"  ", "  ", "  "};
  switch (num_bytes) {
    case 3: sprint_hex(hex[2], data[2]); /* Fallthrough. */
    case 2: sprint_hex(hex[1], data[1]); /* Fallthrough. */
    case 1: sprint_hex(hex[0], data[0]); break;
  }

  snprintf(buffer, size, "%s %s %s  %-15s", hex[0], hex[1], hex[2], temp);
  return num_bytes;
}

Registers reg;

void render_debugview()
{

    glfwPollEvents();
    glfwSetDropCallback(window, drop_callback);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::Begin("DockSpace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar);

    if (g_rom_ptr != nullptr)
    {

        ImGui::Begin("ROM");
        ImGui::Text("Registers");

        ImGui::Text("A: %02X", reg.a);
        ImGui::Text("B: %02X C: %02X BC: %04X", reg.b, reg.c, reg.bc);
        ImGui::Text("D: %02X E: %02X DE: %04X", reg.d, reg.e, reg.de);
        ImGui::Text("H: %02X L: %02X HL: %04X", reg.h, reg.l, reg.hl);
        ImGui::Text("SP: %04X PC: %04X", reg.sp, reg.pc);
        ImGui::Text("Flags: %d%d%d%d", reg.f.z, reg.f.n, reg.f.h, reg.f.c);

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("step")) {
            step();
            reg = get_registers();
        }
        ImGui::PopButtonRepeat();

        static int jumpTo = 0;

        if(ImGui::InputInt("PC", &jumpTo)) {

            set_pc(jumpTo);
            reg.pc = jumpTo;

        }

        s16 offset = -10;

        if (reg.pc < 10)
            offset = 0;

        static ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        char* buffer = new char[MAX_BUFFER];

        disassemble_instr(g_rom_ptr + reg.pc, buffer, MAX_BUFFER);

        ImGui::Text("Current instruction: [PC:%04X] %s", reg.pc, buffer);

        for (int i = 0; i < 20; i++)
        {

            // get the current opcode
            u8 opcode = g_rom_ptr[reg.pc + offset];

            // get the length of the instruction
            u8 length = s_opcode_bytes[opcode];

            disassemble_instr(g_rom_ptr + reg.pc + offset, buffer, MAX_BUFFER);

            if ( reg.pc + offset == reg.pc)
            {
                color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            else
            {
                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            }

            ImGui::TextColored(color, "[0x%04X]: ", reg.pc + offset);
            ImGui::SameLine();
            ImGui::Text("%s", buffer);

            offset += length;
        }
        ImGui::End();
        static MemoryEditor ramView;

        ramView.DrawWindow("RAM", g_mem_ptr, MEMORY_SIZE);
    }
    ImGui::End();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void end_debugview()
{

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}