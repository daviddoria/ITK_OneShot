#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

typedef itk::VectorImage<float, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilename value" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  std::stringstream ss;
  unsigned int counter = 0;
  for(int i = 3; i < argc; ++i)
  {
    ss << argv[i] << " ";
    counter++;
  }

  unsigned int numberOfComponents = counter;

  ImageType::PixelType value(numberOfComponents);

  for(unsigned int i = 0; i < numberOfComponents; ++i)
  {
    ss >> value[i];
  }

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "value " << value << std::endl;

  // Read the image
  typedef  itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();
  
  if(numberOfComponents != reader->GetOutput()->GetNumberOfComponentsPerPixel())
  {
    std::stringstream ss;
    ss << "Number of components is " << numberOfComponents << " but must match the number of components "
       << " in the image, which is " << reader->GetOutput()->GetNumberOfComponentsPerPixel();
   throw std::runtime_error(ss.str());
  }

  typedef itk::Image<unsigned char, 2> MaskType;

  MaskType::Pointer mask = MaskType::New();
  mask->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  mask->Allocate();
  mask->FillBuffer(0);

  typename itk::ImageRegionConstIterator<ImageType> imageIterator(reader->GetOutput(),
                                                             reader->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.Get() == value)
      {
      mask->SetPixel(imageIterator.GetIndex(), 255);
      }
    ++imageIterator;
    }

  // Write the result
  typedef  itk::ImageFileWriter<MaskType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(mask);
  writer->Update();

  return EXIT_SUCCESS;
}
