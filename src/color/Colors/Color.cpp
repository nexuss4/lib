//
// Created by DexrnZacAttack on 1/2/2025.
//

#include "LCE/color/Color.h"
#include "LCE/color/ColorFile.h"

namespace lce::color {
    Color::Color(const ARGB color) : color(color) {}

    Color::Color(std::vector<uint8_t> &data)
        : Color(bio::BinaryBuffer(data.data())) {}

    Color::Color(uint8_t *data) : Color(bio::BinaryBuffer(data)) {}

    Color::Color(bio::BinaryBuffer &&io) : Color(io) {}

    Color::Color(bio::BinaryBuffer &io) { this->color = io.readLE<ARGB>(); }

    size_t Color::getSize() const { return sizeof(ARGB); }

    uint8_t *Color::serialize() const {
        bio::BinaryBuffer io(getSize());

        io.writeLE<ARGB>(color);

        return io.getData();
    }

    ColorCommons::ColorCommons() {}

} // namespace lce::color
