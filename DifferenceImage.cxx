#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkAbsoluteValueDifferenceImageFilter.h"
#include "itkImageRegionIterator.h"

typedef itk::Image<unsigned char, 2>  ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: input1Filename input2Filename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string input1Filename = argv[1];
  std::string input2Filename = argv[2];
  std::string outputFilename = argv[3];

  // Output arguments
  std::cout << "input1Filename " << input1Filename << std::endl;
  std::cout << "input2Filename " << input2Filename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read files
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(input1Filename.c_str());
  reader1->Update();

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(input2Filename.c_str());
  reader2->Update();

  typedef itk::AbsoluteValueDifferenceImageFilter<ImageType, ImageType, ImageType> DifferenceImageFilterType;
  DifferenceImageFilterType::Pointer differenceFilter = DifferenceImageFilterType::New();
  differenceFilter->SetInput(0, reader1->GetOutput());
  differenceFilter->SetInput(1, reader2->GetOutput());
  differenceFilter->Update();
  
  ImageType::Pointer image = ImageType::New();
  ImageType::RegionType region = differenceFilter->GetOutput()->GetLargestPossibleRegion();
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
  
  itk::ImageRegionIterator<ImageType> differenceIterator(differenceFilter->GetOutput(), differenceFilter->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> imageIterator(image, image->GetLargestPossibleRegion());
 
  while(!imageIterator.IsAtEnd())
    {
    if(differenceIterator.Get())
      {
      imageIterator.Set(255);
      }
 
    ++imageIterator;
    ++differenceIterator;
    }

  typedef  itk::ImageFileWriter<ImageType> WriterType;
  /*
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();
  */
  WriterType::Pointer differenceWriter = WriterType::New();
  differenceWriter->SetFileName(outputFilename);
  differenceWriter->SetInput(differenceFilter->GetOutput());
  differenceWriter->Update();

  return EXIT_SUCCESS;
}
