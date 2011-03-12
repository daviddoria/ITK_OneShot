#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRandomImageSource.h"

typedef itk::VectorImage<float, 2> ImageType;
typedef itk::Image< float, 2> ScalarImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilename noiseLevel" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  std::string strNoise = argv[3];

  std::stringstream ssNoise;
  ssNoise << strNoise;
  float noiseLevel;
  ssNoise >> noiseLevel;

  std::cout << "Noise level: " << noiseLevel << std::endl;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  ImageType::Pointer output = ImageType::New();
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->SetNumberOfComponentsPerPixel(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  output->Allocate();
  //output->FillBuffer(itk::NumericTraits<ImageType::PixelType>::Zero);

  // Loop over each component
  std::cout << "Adding noise to " << reader->GetOutput()->GetNumberOfComponentsPerPixel() << " components." << std::endl;
  for(unsigned int component = 0; component < reader->GetOutput()->GetNumberOfComponentsPerPixel(); component++)
    {

    itk::RandomImageSource<ScalarImageType>::Pointer randomImageSource =
      itk::RandomImageSource<ScalarImageType>::New();
    randomImageSource->SetSize(output->GetLargestPossibleRegion().GetSize());
    randomImageSource->SetSpacing(output->GetSpacing());
    randomImageSource->SetOrigin(output->GetOrigin());
    randomImageSource->SetMin(-noiseLevel);
    randomImageSource->SetMax(noiseLevel);
    randomImageSource->Update();

    itk::ImageRegionConstIterator<ScalarImageType> noiseIterator(randomImageSource->GetOutput(),
                                                                 randomImageSource->GetOutput()->GetLargestPossibleRegion());
    itk::ImageRegionConstIterator<ImageType> inputIterator(reader->GetOutput(),
                                                           reader->GetOutput()->GetLargestPossibleRegion());
    itk::ImageRegionIterator<ImageType> outputIterator(output, output->GetLargestPossibleRegion());

    while(!outputIterator.IsAtEnd())
      {
      ImageType::PixelType pixel = outputIterator.Get();
      //std::cout << "Output was: " << pixel << std::endl;
      //std::cout << "Input was: " << inputIterator.Get() << std::endl;
      //std::cout << "Noise is: " << noiseIterator.Get() << std::endl;
      pixel[component] = inputIterator.Get()[component] + noiseIterator.Get();
      //std::cout << "Pixel changed to: " << pixel << std::endl;
      outputIterator.Set(pixel);

      ++outputIterator;
      ++noiseIterator;
      ++inputIterator;
      }
    }

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
