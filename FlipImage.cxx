#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFlipImageFilter.h"
#include "itkFixedArray.h"

typedef itk::Image<itk::CovariantVector<unsigned char, 3>, 2>  RGBImageType;

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  typedef itk::ImageFileReader<RGBImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  typedef itk::FlipImageFilter <RGBImageType> FlipImageFilterType;
  FlipImageFilterType::Pointer flipFilter = FlipImageFilterType::New ();
  flipFilter->SetInput(reader->GetOutput());

  itk::FixedArray<bool, 2> flipAxes;
  flipAxes[0] = false;
  flipAxes[1] = true;
  flipFilter->SetFlipAxes(flipAxes);
  flipFilter->Update();
  
  RGBImageType::PointType origin;
  origin.Fill(0);
  flipFilter->GetOutput()->SetOrigin(origin);

  typedef  itk::ImageFileWriter< RGBImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(flipFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
