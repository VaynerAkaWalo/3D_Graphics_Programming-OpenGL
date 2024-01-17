#include "AbstractMaterial.h"
#include "glm/glm.hpp"

namespace xe {
       class KdMaterial : public AbstractMaterial<KdMaterial> {
        private:
            const glm::vec4 Kd_;
            bool use_vertex_colors = false;
            GLuint texture_;
        public:
            KdMaterial(const glm::vec4 &Kd) : Kd_(Kd), texture_(0) {}

            KdMaterial(const glm::vec4 &Kd, bool use_vertex_colors, GLuint texture) : Kd_(Kd) {
                use_vertex_colors = use_vertex_colors;
                texture_ = texture;
            }

            static void init();
            
            void bind() const override;
            void unbind() const override;
       };
    }

