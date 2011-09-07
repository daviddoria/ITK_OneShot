#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLaplacianImageFilter.h"

typedef itk::Image<float, 2> ScalarImageType;

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
 
  // Read file
  typedef itk::ImageFileReader<ScalarImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();
  
  typedef itk::LaplacianImageFilter<ScalarImageType, ScalarImageType>  LaplacianFilterType;

  // Compute Laplacian
  LaplacianFilterType::Pointer laplacianFilter = LaplacianFilterType::New();
  laplacianFilter->SetInput(reader->GetOutput());
  laplacianFilter->Update();

  // Write the result
  typedef  itk::ImageFileWriter<ScalarImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(laplacianFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
