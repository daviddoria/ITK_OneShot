#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLaplacianImageFilter.h"

// vector image support
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkImageToVectorImageFilter.h"

typedef itk::VectorImage<float, 2> VectorImageType;
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
  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::VectorIndexSelectionCastImageFilter<VectorImageType, ScalarImageType> DisassemblerType;
  typedef itk::ImageToVectorImageFilter<ScalarImageType> ReassemblerType;
  
  ReassemblerType::Pointer reassembler = ReassemblerType::New();
  // Perform the Poisson reconstruction on each channel (source/Laplacian pair) independently
  
  typedef itk::LaplacianImageFilter<ScalarImageType, ScalarImageType>  LaplacianFilterType;
  std::vector<LaplacianFilterType::Pointer> filters;
  
  for(unsigned int component = 0; component < reader->GetOutput()->GetNumberOfComponentsPerPixel(); component++)
    {
    // Disassemble the image into its components
    
    DisassemblerType::Pointer sourceDisassembler = DisassemblerType::New();
    sourceDisassembler->SetIndex(component);
    sourceDisassembler->SetInput(reader->GetOutput());
    sourceDisassembler->Update();
  
    DisassemblerType::Pointer laplacianDisassembler = DisassemblerType::New();
    laplacianDisassembler->SetIndex(component);
    laplacianDisassembler->SetInput(reader->GetOutput());
    laplacianDisassembler->Update();
    
    // Compute Laplacian
    LaplacianFilterType::Pointer laplacianFilter = LaplacianFilterType::New();
    filters.push_back(laplacianFilter);
    laplacianFilter->SetInput(sourceDisassembler->GetOutput());
    laplacianFilter->Update();

    // Reassemble the image
    reassembler->SetNthInput(component, laplacianFilter->GetOutput());
    }

  reassembler->Update();
  
  std::cout << "Writing " << reassembler->GetOutput()->GetNumberOfComponentsPerPixel() << " channel image." << std::endl;

  // Write the result
  typedef  itk::ImageFileWriter<VectorImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(reassembler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
