#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRandomImageSource.h"

typedef itk::Image<unsigned char, 2> ImageType;

void AddNoise(ImageType::Pointer image, float percentToFlip);

int main(int argc, char *argv[])
{
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilename noiseLevel (0.0 = 0% to 1.0 =100%)" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  std::string strNoise = argv[3];

  std::stringstream ssNoise;
  ssNoise << strNoise;
  float noise;
  ssNoise >> noise;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  ImageType::Pointer image = reader->GetOutput();
  image->DisconnectPipeline();

  AddNoise(image, noise);

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();

  return EXIT_SUCCESS;
}

void AddNoise(ImageType::Pointer image, float percentToFlip)
{
  // eg. percentToFlip is .9 if 90% of pixels should be flipped
  typedef itk::Image< float, 2> FloatImageType;
  itk::RandomImageSource<FloatImageType>::Pointer randomImageSource =
    itk::RandomImageSource<FloatImageType>::New();
  randomImageSource->SetNumberOfThreads(1); // to produce non-random results
  randomImageSource->SetSize(image->GetLargestPossibleRegion().GetSize());
  randomImageSource->SetSpacing(image->GetSpacing());
  randomImageSource->SetOrigin(image->GetOrigin());
  randomImageSource->SetMin(0);
  randomImageSource->SetMax(1);
  randomImageSource->Update();

  itk::ImageRegionConstIterator<FloatImageType> noiseImageIterator(randomImageSource->GetOutput(),
                                                                   randomImageSource->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> imageIterator(image, image->GetLargestPossibleRegion());

  unsigned int numberFlipped = 0;
  while(!imageIterator.IsAtEnd())
    {
    //std::cout << "Noise image: " << noiseImageIterator.Get() << std::endl;
    if(noiseImageIterator.Get() < percentToFlip) // we want to flip this pixel
      {
      if(imageIterator.Get() == 0)
        {
        imageIterator.Set(255);
        }
      else if(imageIterator.Get() == 255)
        {
        imageIterator.Set(0);
        }
      ++numberFlipped;
      }
    ++imageIterator;
    ++noiseImageIterator;
    }
  std::cout << "Corrupted " << numberFlipped << " pixels. ("
                                                << 100.*static_cast<float>(numberFlipped)/image->GetLargestPossibleRegion().GetNumberOfPixels()
                                                << " percent)" << std::endl;

}