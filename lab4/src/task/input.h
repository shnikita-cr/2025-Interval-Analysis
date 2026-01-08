#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

struct FileHeader {
    uint8_t side = 0;
    uint8_t mode = 0;
    uint16_t frameCount = 0;
    std::array<uint8_t, 252> reserved{};

    static constexpr size_t SIZE = 256;

    static FileHeader readFrom(std::ifstream &in) {
        std::array<uint8_t, SIZE> raw{};
        readExact(in, raw.data(), raw.size(), "FileHeader(256)");

        FileHeader h;
        h.side = raw[0];
        h.mode = raw[1];
        h.frameCount = readLeU16(raw.data() + 2);
        std::memcpy(h.reserved.data(), raw.data() + 4, h.reserved.size());
        return h;
    }

private:
    static void readExact(std::ifstream &in, void *dst, size_t n, const char *what) {
        in.read(reinterpret_cast<char *>(dst), static_cast<std::streamsize>(n));
        if (!in || static_cast<size_t>(in.gcount()) != n) {
            throw std::runtime_error(std::string("Failed to read ") + what);
        }
    }

    static uint16_t readLeU16(const uint8_t *p) {
        return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
    }
};

struct FrameHeader {
    uint16_t stopPoint = 0;
    uint32_t timeStamp = 0;
    std::array<uint16_t, 5> reserved{};

    static constexpr size_t SIZE = 16;

    static FrameHeader parseFrom16Bytes(const uint8_t *raw16) {
        FrameHeader h;
        h.stopPoint = readLeU16(raw16 + 0);
        h.timeStamp = readLeU32(raw16 + 2);
        for (size_t i = 0; i < 5; ++i) {
            h.reserved[i] = readLeU16(raw16 + 6 + i * 2);
        }
        return h;
    }

private:
    static uint16_t readLeU16(const uint8_t *p) {
        return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
    }

    static uint32_t readLeU32(const uint8_t *p) {
        return static_cast<uint32_t>(p[0])
               | (static_cast<uint32_t>(p[1]) << 8)
               | (static_cast<uint32_t>(p[2]) << 16)
               | (static_cast<uint32_t>(p[3]) << 24);
    }
};

struct MemoryPoint {
    std::array<uint16_t, 8> data{};

    uint16_t raw(size_t channel) const {
        if (channel >= data.size()) throw std::out_of_range("channel");
        return data[channel];
    }

    double volts(size_t channel) const {
        const uint16_t n = raw(channel);
        return static_cast<double>(n) / 16383.0 - 0.5;
    }
};

struct MemoryFrame {
    FrameHeader header{};
    std::array<MemoryPoint, 1024> points{};

    static constexpr size_t POINTS = 1024;
    static constexpr size_t SIZE = FrameHeader::SIZE + POINTS * 8 * 2; // 16400

    static MemoryFrame readFrom(std::ifstream &in) {
        std::array<uint8_t, SIZE> raw{};
        readExact(in, raw.data(), raw.size(), "MemoryFrame(16400)");

        MemoryFrame f;
        f.header = FrameHeader::parseFrom16Bytes(raw.data());

        const uint8_t *p = raw.data() + FrameHeader::SIZE;
        for (size_t i = 0; i < POINTS; ++i) {
            for (size_t ch = 0; ch < 8; ++ch) {
                f.points[i].data[ch] = readLeU16(p);
                p += 2;
            }
        }
        return f;
    }

private:
    static void readExact(std::ifstream &in, void *dst, size_t n, const char *what) {
        in.read(reinterpret_cast<char *>(dst), static_cast<std::streamsize>(n));
        if (!in || static_cast<size_t>(in.gcount()) != n) {
            throw std::runtime_error(std::string("Failed to read ") + what);
        }
    }

    static uint16_t readLeU16(const uint8_t *p) {
        return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
    }
};

struct InputFile {
    FileHeader header{};
    std::vector<MemoryFrame> frames{};

    static InputFile read(const std::string &path) {
        std::ifstream in(path, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open file: " + path);

        InputFile file;
        file.header = FileHeader::readFrom(in);

        file.validateHeader();
        file.readAllFrames(in);
        return file;
    }

    const MemoryFrame &frame(size_t index) const {
        if (index >= frames.size()) throw std::out_of_range("frame index");
        return frames[index];
    }

    std::vector<double> flattenVolts() const {
        const size_t total =
                frames.size() * MemoryFrame::POINTS * 8;

        std::vector<double> out;
        out.reserve(total);

        for (const auto &fr: frames) {
            for (const auto &pt: fr.points) {
                for (size_t ch = 0; ch < pt.data.size(); ++ch) {
                    const uint16_t n = pt.data[ch];
                    out.push_back(static_cast<double>(n) / 16383.0 - 0.5);
                }
            }
        }
        return out;
    }

private:
    void validateHeader() const {
        if (header.side > 1) throw std::runtime_error("Bad header.side");
        if (header.mode > 1) throw std::runtime_error("Bad header.mode");
    }

    void readAllFrames(std::ifstream &in) {
        frames.clear();
        frames.reserve(header.frameCount);
        for (uint16_t i = 0; i < header.frameCount; ++i) {
            frames.push_back(MemoryFrame::readFrom(in));
        }
    }
};
