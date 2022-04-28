#include "Grid.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iterator>

namespace xyz {

namespace {

constexpr float ndc_width = 2.0f;
constexpr float ndc_height = 2.0f;
constexpr float y_base = -1.0f;
constexpr float z_base = 1.0f;

const std::array<std::array<float, 4>, 128> CLUT = {{
    {0.050382f, 0.029802f, 0.527975f, 1.0f},
    {0.075442f, 0.027196f, 0.538044f, 1.0f},
    {0.096535f, 0.025150f, 0.547171f, 1.0f},
    {0.115337f, 0.023539f, 0.555562f, 1.0f},
    {0.132643f, 0.022239f, 0.563366f, 1.0f},
    {0.148918f, 0.021133f, 0.570700f, 1.0f},
    {0.164428f, 0.020147f, 0.577635f, 1.0f},
    {0.179354f, 0.019226f, 0.584229f, 1.0f},
    {0.193823f, 0.018324f, 0.590521f, 1.0f},
    {0.207929f, 0.017407f, 0.596539f, 1.0f},
    {0.221736f, 0.016456f, 0.602302f, 1.0f},
    {0.235297f, 0.015455f, 0.607823f, 1.0f},
    {0.248658f, 0.014384f, 0.613109f, 1.0f},
    {0.261854f, 0.013245f, 0.618160f, 1.0f},
    {0.274906f, 0.012038f, 0.622977f, 1.0f},
    {0.287836f, 0.010777f, 0.627554f, 1.0f},
    {0.300659f, 0.009475f, 0.631885f, 1.0f},
    {0.313392f, 0.008147f, 0.635960f, 1.0f},
    {0.326043f, 0.006819f, 0.639768f, 1.0f},
    {0.338620f, 0.005521f, 0.643297f, 1.0f},
    {0.351131f, 0.004286f, 0.646536f, 1.0f},
    {0.363578f, 0.003153f, 0.649469f, 1.0f},
    {0.375966f, 0.002166f, 0.652083f, 1.0f},
    {0.388295f, 0.001371f, 0.654362f, 1.0f},
    {0.400566f, 0.000819f, 0.656293f, 1.0f},
    {0.412777f, 0.000568f, 0.657860f, 1.0f},
    {0.424927f, 0.000677f, 0.659052f, 1.0f},
    {0.437012f, 0.001207f, 0.659855f, 1.0f},
    {0.449032f, 0.002220f, 0.660256f, 1.0f},
    {0.460980f, 0.003786f, 0.660246f, 1.0f},
    {0.472851f, 0.005975f, 0.659815f, 1.0f},
    {0.484641f, 0.008855f, 0.658958f, 1.0f},
    {0.496343f, 0.012498f, 0.657669f, 1.0f},
    {0.507952f, 0.016969f, 0.655944f, 1.0f},
    {0.519463f, 0.022343f, 0.653786f, 1.0f},
    {0.530866f, 0.028687f, 0.651195f, 1.0f},
    {0.542158f, 0.036067f, 0.648178f, 1.0f},
    {0.553330f, 0.044366f, 0.644743f, 1.0f},
    {0.564377f, 0.052819f, 0.640901f, 1.0f},
    {0.575294f, 0.061345f, 0.636667f, 1.0f},
    {0.586074f, 0.069940f, 0.632055f, 1.0f},
    {0.596712f, 0.078592f, 0.627086f, 1.0f},
    {0.607203f, 0.087298f, 0.621782f, 1.0f},
    {0.617546f, 0.096049f, 0.616164f, 1.0f},
    {0.627734f, 0.104836f, 0.610260f, 1.0f},
    {0.637767f, 0.113655f, 0.604093f, 1.0f},
    {0.647642f, 0.122500f, 0.597690f, 1.0f},
    {0.657359f, 0.131364f, 0.591079f, 1.0f},
    {0.666916f, 0.140242f, 0.584287f, 1.0f},
    {0.676316f, 0.149131f, 0.577339f, 1.0f},
    {0.685558f, 0.158025f, 0.570260f, 1.0f},
    {0.694644f, 0.166923f, 0.563076f, 1.0f},
    {0.703578f, 0.175822f, 0.555807f, 1.0f},
    {0.712362f, 0.184719f, 0.548475f, 1.0f},
    {0.720998f, 0.193614f, 0.541098f, 1.0f},
    {0.729489f, 0.202504f, 0.533695f, 1.0f},
    {0.737841f, 0.211391f, 0.526281f, 1.0f},
    {0.746056f, 0.220274f, 0.518869f, 1.0f},
    {0.754138f, 0.229153f, 0.511469f, 1.0f},
    {0.762092f, 0.238030f, 0.504091f, 1.0f},
    {0.769920f, 0.246906f, 0.496741f, 1.0f},
    {0.777628f, 0.255781f, 0.489428f, 1.0f},
    {0.785216f, 0.264659f, 0.482157f, 1.0f},
    {0.792690f, 0.273540f, 0.474927f, 1.0f},
    {0.800052f, 0.282429f, 0.467742f, 1.0f},
    {0.807304f, 0.291326f, 0.460602f, 1.0f},
    {0.814450f, 0.300235f, 0.453510f, 1.0f},
    {0.821489f, 0.309159f, 0.446461f, 1.0f},
    {0.828427f, 0.318101f, 0.439455f, 1.0f},
    {0.835260f, 0.327063f, 0.432487f, 1.0f},
    {0.841992f, 0.336050f, 0.425560f, 1.0f},
    {0.848622f, 0.345065f, 0.418666f, 1.0f},
    {0.855151f, 0.354111f, 0.411802f, 1.0f},
    {0.861578f, 0.363191f, 0.404967f, 1.0f},
    {0.867900f, 0.372311f, 0.398155f, 1.0f},
    {0.874119f, 0.381474f, 0.391362f, 1.0f},
    {0.880232f, 0.390682f, 0.384585f, 1.0f},
    {0.886236f, 0.399941f, 0.377822f, 1.0f},
    {0.892129f, 0.409253f, 0.371068f, 1.0f},
    {0.897907f, 0.418622f, 0.364320f, 1.0f},
    {0.903568f, 0.428053f, 0.357576f, 1.0f},
    {0.909109f, 0.437549f, 0.350831f, 1.0f},
    {0.914526f, 0.447113f, 0.344085f, 1.0f},
    {0.919813f, 0.456748f, 0.337335f, 1.0f},
    {0.924967f, 0.466459f, 0.330579f, 1.0f},
    {0.929983f, 0.476248f, 0.323816f, 1.0f},
    {0.934857f, 0.486119f, 0.317045f, 1.0f},
    {0.939582f, 0.496074f, 0.310266f, 1.0f},
    {0.944155f, 0.506115f, 0.303477f, 1.0f},
    {0.948570f, 0.516247f, 0.296681f, 1.0f},
    {0.952822f, 0.526470f, 0.289876f, 1.0f},
    {0.956903f, 0.536787f, 0.283063f, 1.0f},
    {0.960810f, 0.547200f, 0.276247f, 1.0f},
    {0.964538f, 0.557711f, 0.269428f, 1.0f},
    {0.968078f, 0.568321f, 0.262609f, 1.0f},
    {0.971425f, 0.579031f, 0.255793f, 1.0f},
    {0.974574f, 0.589844f, 0.248985f, 1.0f},
    {0.977520f, 0.600761f, 0.242192f, 1.0f},
    {0.980255f, 0.611780f, 0.235421f, 1.0f},
    {0.982774f, 0.622904f, 0.228682f, 1.0f},
    {0.985068f, 0.634136f, 0.221979f, 1.0f},
    {0.987131f, 0.645473f, 0.215330f, 1.0f},
    {0.988958f, 0.656916f, 0.208749f, 1.0f},
    {0.990541f, 0.668466f, 0.202257f, 1.0f},
    {0.991874f, 0.680123f, 0.195873f, 1.0f},
    {0.992948f, 0.691888f, 0.189625f, 1.0f},
    {0.993756f, 0.703759f, 0.183545f, 1.0f},
    {0.994290f, 0.715738f, 0.177668f, 1.0f},
    {0.994542f, 0.727824f, 0.172040f, 1.0f},
    {0.994506f, 0.740015f, 0.166709f, 1.0f},
    {0.994171f, 0.752313f, 0.161732f, 1.0f},
    {0.993530f, 0.764717f, 0.157172f, 1.0f},
    {0.992569f, 0.777228f, 0.153088f, 1.0f},
    {0.991286f, 0.789841f, 0.149563f, 1.0f},
    {0.989676f, 0.802554f, 0.146668f, 1.0f},
    {0.987719f, 0.815372f, 0.144458f, 1.0f},
    {0.985419f, 0.828287f, 0.142998f, 1.0f},
    {0.982763f, 0.841298f, 0.142321f, 1.0f},
    {0.979755f, 0.854401f, 0.142441f, 1.0f},
    {0.976375f, 0.867599f, 0.143317f, 1.0f},
    {0.972637f, 0.880883f, 0.144875f, 1.0f},
    {0.968543f, 0.894247f, 0.146962f, 1.0f},
    {0.964110f, 0.907685f, 0.149323f, 1.0f},
    {0.959352f, 0.921194f, 0.151533f, 1.0f},
    {0.954347f, 0.934746f, 0.152909f, 1.0f},
    {0.949192f, 0.948326f, 0.152190f, 1.0f},
    {0.944172f, 0.961861f, 0.146889f, 1.0f},
    {0.940015f, 0.975156f, 0.131326f, 1.0f},
}};

} // namespace

Grid::Grid(size_t i, size_t j) : i_(i), j_(j) {
  vertices_.resize(i_ * j_ * TrianglesInPyramide);
  colors_.resize(i_ * j_ * TrianglesInPyramide);
  buffers_.resize(NumBuffers, 0);
}

Grid::Grid(Grid &&other) : i_(other.i_), j_(other.j_) {
  *this = std::move(other);
}

Grid::~Grid() {
  if (!buffers_.empty()) {
    glDeleteBuffers(buffers_.size(), buffers_.data());
  }
}

Grid &Grid::operator=(Grid &&other) {
  buffers_ = std::move(other.buffers_);
  vertices_ = std::move(other.vertices_);
  colors_ = std::move(other.colors_);

  return *this;
}

void Grid::init(GLuint *array) {
  const float width = ndc_width / i_;
  const float hwidth = width / 2;
  const float height = sqrt(width * width - hwidth * hwidth);

  float x = -1.0f;
  for (size_t i = 0; i < i_; ++i) {
    float z = z_base;

    for (size_t j = 0; j < j_; ++j) {
      const size_t index = (i * j_ + j) * TrianglesInPyramide;

      vertices_[index + 0][0] = {x, y_base, z};
      vertices_[index + 0][1] = {x + width, y_base, z};
      vertices_[index + 0][2] = {x + hwidth, y_base + height, z + hwidth};

      vertices_[index + 1][0] = {x + width, y_base, z};
      vertices_[index + 1][1] = {x + width, y_base, z + width};
      vertices_[index + 1][2] = {x + hwidth, y_base + height, z + hwidth};

      vertices_[index + 2][0] = {x, y_base, z + width};
      vertices_[index + 2][1] = {x + width, y_base, z + width};
      vertices_[index + 2][2] = {x + hwidth, y_base + height, z + hwidth};

      vertices_[index + 3][0] = {x, y_base, z + width};
      vertices_[index + 3][1] = {x, y_base, z};
      vertices_[index + 3][2] = {x + hwidth, y_base + height, z + hwidth};

      colors_[index + 0][0] = CLUT[0];
      colors_[index + 0][1] = CLUT[0];
      colors_[index + 0][2] = CLUT[15];

      colors_[index + 1][0] = CLUT[0];
      colors_[index + 1][1] = CLUT[0];
      colors_[index + 1][2] = CLUT[15];

      colors_[index + 2][0] = CLUT[0];
      colors_[index + 2][1] = CLUT[0];
      colors_[index + 2][2] = CLUT[15];

      colors_[index + 3][0] = CLUT[0];
      colors_[index + 3][1] = CLUT[0];
      colors_[index + 3][2] = CLUT[15];

      z += width;
    }
    x += width;
  }

  glCreateBuffers(buffers_.size(), buffers_.data());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER,
               i_ * j_ * TrianglesInPyramide * sizeof(TriangleVertices),
               vertices_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, VertexComponents, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[ColorBuffer]);
  glBufferData(GL_ARRAY_BUFFER,
               i_ * j_ * TrianglesInPyramide * sizeof(TriangleColors),
               colors_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, ColorComponents, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void Grid::display(float *values) {
  for (size_t i = 0; i < i_; ++i) {
    for (size_t j = 0; j < j_; ++j) {
      const size_t index = (i * j_ + j) * TrianglesInPyramide;

      const float value = values[i * j_ + j];
      const float nvalue =
          std::clamp(static_cast<int>(floor(value * CLUT.size())), 0,
                     static_cast<int>(CLUT.size() - 1));

      vertices_[index + 0][2][1] = y_base + value;
      vertices_[index + 1][2][1] = y_base + value;
      vertices_[index + 2][2][1] = y_base + value;
      vertices_[index + 3][2][1] = y_base + value;

      colors_[index + 0][2] = CLUT[nvalue];
      colors_[index + 1][2] = CLUT[nvalue];
      colors_[index + 2][2] = CLUT[nvalue];
      colors_[index + 3][2] = CLUT[nvalue];
    }
  }

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER,
               i_ * j_ * TrianglesInPyramide * sizeof(TriangleVertices),
               vertices_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, VertexComponents, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[ColorBuffer]);
  glBufferData(GL_ARRAY_BUFFER,
               i_ * j_ * TrianglesInPyramide * sizeof(TriangleColors),
               colors_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, ColorComponents, GL_FLOAT, GL_FALSE, 0, nullptr);
}

size_t Grid::num_vertices() const {
  return vertices_.size() * TrianglesInPyramide * 3;
}

} // namespace xyz
