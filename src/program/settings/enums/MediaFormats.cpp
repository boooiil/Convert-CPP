#include "MediaFormats.h"

const ObjectEnumDataHolder<MediaFormat> MediaFormats::CUSTOM("custom",
                                                             MediaFormat());

const ObjectEnumDataHolder<MediaFormat> MediaFormats::P_480(
    "480p",
    MediaFormat("480p", 24, 0.6f, 0.4f, 0.8f, 854, 480, "854:356", "854:480"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::PC_480(
    "480pc", MediaFormat("480pc", 24, 0.6f, 0.4f, 0.8f, 1138, 640, "854:720",
                         "1138:640"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::P_720(
    "720p", MediaFormat("720p", 24, 1.4f, 1.2f, 1.8f, 1280, 720, "1280:534",
                        "1280:720"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::PM_720(
    "720pm", MediaFormat("720pm", 24, 1.4f, 1.2f, 1.8f, 1280, 720, "1280:580",
                         "1280:720"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::PN_720(
    "720pn", MediaFormat("720pn", 24, 1.4f, 1.2f, 1.8f, 1280, 720, "1280:640",
                         "1280:720"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::P_1080(
    "1080p", MediaFormat("1080p", 24, 2.0f, 1.6f, 2.2f, 1920, 1080, "1920:800",
                         "1920:1080"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::PM_1080(
    "1080pm", MediaFormat("1080pm", 24, 2.0f, 1.6f, 2.2f, 1920, 1080,
                          "1920:870", "1920:1080"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::PN_1080(
    "1080pn", MediaFormat("1080pn", 24, 2.0f, 1.6f, 2.2f, 1920, 1080,
                          "1920:960", "1920:1080"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::P_1440(
    "1440p", MediaFormat("1440p", 24, 20.0f, 20.0f, 27.0f, 2560, 1440,
                         "2560:1068", "2560:1440"));

const ObjectEnumDataHolder<MediaFormat> MediaFormats::P_2160(
    "2160p", MediaFormat("2160p", 24, 30.0f, 30.0f, 40.0f, 3840, 2160,
                         "3840:1600", "3840: 2160"));