#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"

//typedef itk::Image<itk::CovariantVector<unsigned char, 3>, 2>  RGBImageType;
typedef itk::Image<unsigned char, 2>  ImageType;

int main(int argc, char *argv[])
{
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename maskFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string maskFilename = argv[2];
  std::string outputFilename = argv[3];

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer imageReader = ReaderType::New();
  imageReader->SetFileName(inputFilename.c_str());
  imageReader->Update();

  ImageType::Pointer outputImage = ImageType::New();
  outputImage->Graft(imageReader->GetOutput());

  ReaderType::Pointer maskReader = ReaderType::New();
  maskReader->SetFileName(maskFilename.c_str());
  maskReader->Update();

  itk::ImageRegionIterator<ImageType> imageIterator(outputImage,outputImage->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> maskIterator(maskReader->GetOutput(),maskReader->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    if(maskIterator.Get())
      {
      imageIterator.Set(255);
      }

    ++imageIterator;
    ++maskIterator;
    }

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(outputImage);
  writer->Update();

  return EXIT_SUCCESS;
}
