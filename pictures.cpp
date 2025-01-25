#include "pictures.hpp"
#include "readconfig.hpp"
#include <SFML/Graphics.hpp>
#include <exiv2/exiv2.hpp>
#include <iostream>

sf::Image rotateImage(const sf::Image& image, int angle) {
    sf::Image rotatedImage;
    if (angle == 90 || angle == 270) {
        rotatedImage.create(image.getSize().y, image.getSize().x);
    } else {
        rotatedImage.create(image.getSize().x, image.getSize().y);
    }

    for (unsigned int y = 0; y < image.getSize().y; ++y) {
        for (unsigned int x = 0; x < image.getSize().x; ++x) {
            if (angle == 90) {
                rotatedImage.setPixel(image.getSize().y - y - 1, x, image.getPixel(x, y));
            } else if (angle == 180) {
                rotatedImage.setPixel(image.getSize().x - x - 1, image.getSize().y - y - 1, image.getPixel(x, y));
            } else if (angle == 270) {
                rotatedImage.setPixel(y, image.getSize().x - x - 1, image.getPixel(x, y));
            } else {
                rotatedImage.setPixel(x, y, image.getPixel(x, y));
            }
        }
    }

    return rotatedImage;
}

sf::RectangleShape createPixelImage(const std::string& imagePath) {
    UnScreenSaverConfig imgConfig = getConfigFromYaml();
    const int h = imgConfig.height;
    const int w = imgConfig.width;

    sf::Image image;
    if (!image.loadFromFile(imagePath)) {
        std::cerr << "Error loading image: " << imagePath << std::endl;
        return sf::RectangleShape();
    }

    // Read EXIF metadata to determine the correct orientation
    Exiv2::Image::UniquePtr exivImage = Exiv2::ImageFactory::open(imagePath);
    exivImage->readMetadata();
    Exiv2::ExifData &exifData = exivImage->exifData();
    int orientation = exifData["Exif.Image.Orientation"].toInt64();

    // Rotate the image based on the orientation
    int angle = 0;
    switch (orientation) {
        case 3:
            angle = 180;
            break;
        case 6:
            angle = 90;
            break;
        case 8:
            angle = 270;
            break;
    }
    if (angle != 0) {
        image = rotateImage(image, angle);
    }

    // Check if image width is greater than height
    sf::Image resizedImage;
    if (image.getSize().x > image.getSize().y) {
        resizedImage.create(h, w);
    } else {
        resizedImage.create(w, h);
    }

    // Resize the image to the specified width and height
    for (unsigned int y = 0; y < resizedImage.getSize().y; ++y) {
        for (unsigned int x = 0; x < resizedImage.getSize().x; ++x) {
            unsigned int srcX = x * image.getSize().x / resizedImage.getSize().x;
            unsigned int srcY = y * image.getSize().y / resizedImage.getSize().y;
            resizedImage.setPixel(x, y, image.getPixel(srcX, srcY));
        }
    }

    sf::Texture* texture = new sf::Texture();
    texture->loadFromImage(resizedImage);

    sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(resizedImage.getSize().x), static_cast<float>(resizedImage.getSize().y)));
    rectangle.setTexture(texture);

    return rectangle;
}