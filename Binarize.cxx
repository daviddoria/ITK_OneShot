#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVectorImage.h"
#include "itkVectorMagnitudeImageFilter.h"

typedef itk::VectorImage<float, 2> VectorImageType;
//typedef itk::Image<float, 2> ScalarImageType;
typedef itk::Image<unsigned char, 2> ScalarImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read file - the conversion from float to unsigned char is done during this step
  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  ScalarImageType::Pointer output = ScalarImageType::New();
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->Allocate();
    
  itk::ImageRegionIterator<VectorImageType> imageIterator(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());
 
  unsigned int validPixels = 0;
  
  while(!imageIterator.IsAtEnd())
    {
    //if(imageIterator.Get().GetNorm() > 0)
    if(imageIterator.Get()[0] > 0)
      {
      output->SetPixel(imageIterator.GetIndex(), 255);
      validPixels++;
      }
    else
      {
      output->SetPixel(imageIterator.GetIndex(), 0);
      }
 
    ++imageIterator;
    }
    
  std::cout << "There were " << validPixels << " valid pixels." << std::endl;

  // Write the result
  typedef  itk::ImageFileWriter<ScalarImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
