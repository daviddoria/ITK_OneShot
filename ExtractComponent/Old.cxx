#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkImageRegionIterator.h"

typedef itk::Image<float, 2>  ScalarImageType;

typedef itk::Image<itk::CovariantVector<float, 2>, 2>  ImageType2;
typedef itk::Image<itk::CovariantVector<float, 3>, 2>  ImageType3;
typedef itk::Image<itk::CovariantVector<float, 4>, 2>  ImageType4;
typedef itk::Image<itk::CovariantVector<float, 5>, 2>  ImageType5;

// Simply reading the file into a single channel image will do the conversion
// If you write it out as a png, it will still be 3 channel - you need to write it out as a mhd

template<typename TImage>
void ExtractComponent(typename TImage::Pointer input, ScalarImageType::Pointer output, unsigned int component)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();
  output->FillBuffer(0);

  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ScalarImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    //std::cout << "Value " << inputIterator.Get()[component] << std::endl;
    std::cout << inputIterator.Get()[0] << " " << inputIterator.Get()[1] << " " << inputIterator.Get()[2] << " " << inputIterator.Get()[3] << std::endl;
    outputIterator.Set(inputIterator.Get()[component]);

    ++inputIterator;
    ++outputIterator;
    }
}

int main(int argc, char *argv[])
{
  if(argc < 5)
    {
    std::cerr << "Required: inputFilename componentNumber outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string numberOfInputComponents = argv[2];
  std::string strComponent = argv[3];
  std::string outputFilename = argv[4];

  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "numberOfInputComponents " << numberOfInputComponents << std::endl;
  std::cout << "strComponent " << strComponent << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  std::stringstream ssInputComponents;
  ssInputComponents << strComponent;
  int inputComponents;
  ssInputComponents >> inputComponents;

  std::stringstream ss;
  ss << strComponent;
  int component;
  ss >> component;

  ScalarImageType::Pointer output = ScalarImageType::New();

  switch(inputComponents)
  {
    case 2:
    {
      typedef itk::ImageFileReader<ImageType2> ReaderType2;
      ReaderType2::Pointer reader = ReaderType2::New();
      reader->SetFileName(inputFilename);
      reader->Update();

      ExtractComponent<ImageType2>(reader->GetOutput(), output, component);
      break;
    }
    case 3:
    {
      typedef itk::ImageFileReader<ImageType3> ReaderType3;
      ReaderType3::Pointer reader = ReaderType3::New();
      reader->SetFileName(inputFilename);
      reader->Update();
      ExtractComponent<ImageType3>(reader->GetOutput(), output, component);
      break;
    }
    case 4:
    {
      typedef itk::ImageFileReader<ImageType4> ReaderType4;
      ReaderType4::Pointer reader = ReaderType4::New();
      reader->SetFileName(inputFilename);
      reader->Update();
      ExtractComponent<ImageType4>(reader->GetOutput(), output, component);
      break;
    }
    case 5:
    {
      typedef itk::ImageFileReader<ImageType5> ReaderType5;
      ReaderType5::Pointer reader = ReaderType5::New();
      reader->SetFileName(inputFilename);
      reader->Update();
      ExtractComponent<ImageType5>(reader->GetOutput(), output, component);
      break;
    }
    default:
      std::cerr << "Input images with " << inputComponents << " are not allowed!" << std::endl;
      return EXIT_FAILURE;
  }

  typedef  itk::ImageFileWriter<ScalarImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  std::cout << "Wrote output." << std::endl;

  return EXIT_SUCCESS;
}
