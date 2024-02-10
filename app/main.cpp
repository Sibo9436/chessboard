#include "chess.hpp"
#include <glad/glad.h>
// Commento assolutamente fondamentale
#include <GLFW/glfw3.h>
// altrimenti l'LSP me li inverte
#include <glm/glm.hpp>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <renderer.hpp>
#include <stb_image.h>

GLFWwindow *initialize_window() {
  // Init GLFW
  if (!glfwInit())
    exit(EXIT_FAILURE);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(640, 480, "Hello GLFW!", NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cerr << "Non è stato possibile creare la finestra\n";
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  // Init GLAD
  if (!gladLoadGL()) {
    // Mi pare che google abbia un'ottima libreria di logging per c++
    std::cout << "Qualcosa è andato storto nell'inizializzazione di glad\n";
    exit(EXIT_FAILURE);
  }

  return window;
}
void init_imgui(GLFWwindow *window) {
  /* IMGUI SETUP */
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

void drawPieces(ChessRenderer &renderer, ChessBoard &board) {
  for (auto piece : *board.getPieces()) {
    renderer.drawPiece(piece->x, piece->y, piece->type, piece->color);
  }
}

// temporaneo
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

  auto board = static_cast<ChessBoard *>(glfwGetWindowUserPointer(window));
  if (board != nullptr && action == GLFW_RELEASE) {
    switch (key) {
    case GLFW_KEY_W:
      board->moveSelected(0, 1);
      break;
    case GLFW_KEY_S:
      board->moveSelected(0, -1);
      break;
    case GLFW_KEY_A:
      board->moveSelected(-1, 0);
      break;
    case GLFW_KEY_D:
      board->moveSelected(1, 0);
      break;
    case GLFW_KEY_N:
      board->select(4, 1);
    }
  }
}
void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    my = height - my;
    int ws = width / 8;
    int hs = height / 8;
    int x = (mx / ws);
    int y = (my / hs);
    auto *board = static_cast<ChessBoard *>(glfwGetWindowUserPointer(window));
    board->select(x, y);
  }
}

int main() {
  std::cout << "Ciao Cmake" << std::endl;
  GLFWwindow *window = initialize_window();
  init_imgui(window);
  glClearColor(0.4f, 0.3f, 0.4f, 0.0f);

  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // GLuint vao = create_test_buffers();
  // GLuint shader_program =
  // create_shader_program("../assets/shaders/test.vert","../assets/shaders/test.frag");
  // std::cout << "Vao: " << vao << " shader_program: "<<shader_program<<
  // std::endl; gpu::QuadRenderer renderer;

  // gpu::BatchRenderer renderer;

  /*  unsigned char blue[8 * 8 * 3] = {0};
    for (size_t i = 2; i < 8 * 8 * 3; i += 3) {
      blue[i] = 255;
    }

    GLuint txtr = renderer.push_texture(blue);
    renderer.push_quad(20, 40, 20, 20, txtr);
    unsigned char green[8 * 8 * 3] = {0};
    for (size_t i = 1; i < 8 * 8 * 3; i += 3) {
      green[i] = 255;
    }
    GLuint greentxtr = renderer.push_texture(green);
    renderer.push_quad(50, 40, 80, 80, txtr);
    renderer.push_quad(50, 40 + 80, 80, 80, greentxtr);
    renderer.push_quad(50 + 80, 40, 80, 80, txtr);
    renderer.push_quad(50 + 80, 40 + 80, 80, 80, greentxtr);
    renderer.push_quad(50, 40 + 160, 80, 80, txtr);
    renderer.push_quad(50 + 160, 40, 80, 80, txtr);
    renderer.push_quad(50 + 160, 40 + 160, 80, 80, greentxtr);
    std::cout << "blue: " << txtr << " gree: " << greentxtr << '\n';
    std::cout << renderer.to_string() << '\n';
    */
  ChessRenderer boardRenderer;

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *image = stbi_load("../assets/chesspieces.png", &width, &height,
                                   &nrChannels, STBI_rgb_alpha);
  std::cout << "Image has " << nrChannels << " channels, width: " << width
            << " height: " << height << "\n";
  if (!image) {
    std::cerr << "Failed to load texture atlas!\n";
  }
  boardRenderer.setTextureAtlas(image, width, height, nrChannels);
  boardRenderer.drawBoard({});
  stbi_image_free(image);
  ChessBoard board;
  glfwSetWindowUserPointer(window, (void *)&board);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_callback);

  while (!glfwWindowShouldClose(window)) {
    /* Poll for and process events */

    // se la memoria non mi inganna gli eventi glfw li
    // gestisce a colpi di callback
    glfwPollEvents();
    // glfwWaitEvents();

    /*other logic here*/
    drawPieces(boardRenderer, board);

    /*Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // render(shader_program, vao);
    // boardRenderer.clearRender();
    boardRenderer.drawBoard(board.possibleMoves());
    boardRenderer.clearRender();

    // ImGui Render here
    /*
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (show_demo_window)
      ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Render();
    ImGui::EndFrame();
    */
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /*Swap front and back buffers */
    glfwSwapBuffers(window);
  }
  /*Cleanup*/
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  // cleanup(shader_program,&vao);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
