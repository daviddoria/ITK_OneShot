// ITK
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"

// Submodules
#include <Helpers/Helpers.h>
#include <ITKHelpers/ITKHelpers.h>

typedef itk::VectorImage<float, 2> ImageType;
typedef itk::Image<unsigned char, 2> PixelImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: Image.png Pixels.png output.txt" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string imageFilename = argv[1];
  std::string pixelsFilename = argv[2];
  std::string outputFilename = argv[3];

  // Output arguments
  std::cout << "imageFilename " << imageFilename << std::endl;
  std::cout << "pixelsFilename " << pixelsFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read files
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFilename);
  reader->Update();

  typedef itk::ImageFileReader<PixelImageType> PixelReaderType;
  PixelReaderType::Pointer pixelReader = PixelReaderType::New();
  pixelReader->SetFileName(pixelsFilename);
  pixelReader->Update();

  std::ofstream fout(outputFilename.c_str());

  itk::ImageRegionConstIteratorWithIndex<PixelImageType> pixelImageIterator(pixelReader->GetOutput(),pixelReader->GetOutput()->GetLargestPossibleRegion());

  while(!pixelImageIterator.IsAtEnd())
  {
    if(pixelImageIterator.Get())
    {
//      fout << reader->GetOutput()->GetPixel(pixelImageIterator.GetIndex()) << std::endl;
      ImageType::PixelType pixel = reader->GetOutput()->GetPixel(pixelImageIterator.GetIndex());
      fout << pixel[0] << " " << pixel[1] << " " << pixel[2] << std::endl;
    }
    ++pixelImageIterator;
  }

  fout.close();

  return EXIT_SUCCESS;
}
