#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

typedef itk::VectorImage<float, 2>  ImageType;

// This program takes two images (N channels and M channels) and produces an N+M channel image as output.
// The N channels of image 1 are the first channels of the output, followed by the M channels of image2.
int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: image1Filename image2Filename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string image1Filename = argv[1];
  std::string image2Filename = argv[2];
  std::string outputFilename = argv[3];
  std::cout << "Appending " << image1Filename << " and " << image2Filename << " into " << outputFilename << std::endl;

  // Read images
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer image1Reader = ReaderType::New();
  image1Reader->SetFileName(image1Filename.c_str());
  image1Reader->Update();

  ReaderType::Pointer image2Reader = ReaderType::New();
  image2Reader->SetFileName(image2Filename.c_str());
  image2Reader->Update();

  if(image1Reader->GetOutput()->GetLargestPossibleRegion() != image2Reader->GetOutput()->GetLargestPossibleRegion())
    {
    std::cerr << "Images must be the same size!" << std::endl;
    std::cerr << "Image1 is " << image1Reader->GetOutput()->GetLargestPossibleRegion() << std::endl;
    std::cerr << "Image2 is " << image2Reader->GetOutput()->GetLargestPossibleRegion() << std::endl;
    exit(-1);
    }

  std::cout << "Image1 has " << image1Reader->GetOutput()->GetNumberOfComponentsPerPixel() << " components." << std::endl;
  std::cout << "Image2 has " << image2Reader->GetOutput()->GetNumberOfComponentsPerPixel() << " components." << std::endl;

  // Create output image
  itk::ImageRegion<2> region = image1Reader->GetOutput()->GetLargestPossibleRegion();

  unsigned int newPixelLength = image1Reader->GetOutput()->GetNumberOfComponentsPerPixel() +
                                image2Reader->GetOutput()->GetNumberOfComponentsPerPixel();

  std::cout << "Output image has " << newPixelLength << " components." << std::endl;

  ImageType::Pointer output = ImageType::New();
  output->SetNumberOfComponentsPerPixel(newPixelLength);
  output->SetRegions(region);
  output->Allocate();

  // Combine images
  itk::ImageRegionConstIterator<ImageType> image1Iterator(image1Reader->GetOutput(), region);
  itk::ImageRegionConstIterator<ImageType> image2Iterator(image2Reader->GetOutput(), region);
  itk::ImageRegionIterator<ImageType> outputIterator(output, region);

  while(!image1Iterator.IsAtEnd())
    {
    ImageType::PixelType pixel;
    pixel.SetSize(newPixelLength);
    //std::cout << "Pixel has " << pixel.GetSize() << " components." << std::endl;

    for(unsigned int i = 0; i < image1Reader->GetOutput()->GetNumberOfComponentsPerPixel(); i++)
      {
      pixel[i] = image1Iterator.Get()[i];
      }

    for(unsigned int i = 0; i < image2Reader->GetOutput()->GetNumberOfComponentsPerPixel(); i++)
      {
      pixel[image1Reader->GetOutput()->GetNumberOfComponentsPerPixel() + i] = image2Iterator.Get()[i];
      }

    outputIterator.Set(pixel);

    ++image1Iterator;
    ++image2Iterator;
    ++outputIterator;
    }

  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
