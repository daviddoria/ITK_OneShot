#include "itkFlipImageFilter.h"
#include "itkFixedArray.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVectorImage.h"

#include "ITKHelpers/ITKHelpers.h"

typedef itk::VectorImage<float, 2> ImageType;

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  typedef itk::FlipImageFilter<ImageType> FlipImageFilterType;
  FlipImageFilterType::Pointer flipFilter = FlipImageFilterType::New ();
  flipFilter->SetInput(reader->GetOutput());

  itk::FixedArray<bool, 2> flipAxes;
  flipAxes[0] = false;
  flipAxes[1] = true;
  flipFilter->SetFlipAxes(flipAxes);
  flipFilter->Update();

  ImageType::PointType origin;
  origin.Fill(0);
  flipFilter->GetOutput()->SetOrigin(origin);

  bool ucharType = ITKHelpers::GetPixelTypeFromFile(inputFilename) == itk::ImageIOBase::UCHAR;
  unsigned int components = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  if( (ucharType && components == 3) ||
      (ucharType && components == 1))
  {
    ITKHelpers::WriteRGBImage(flipFilter->GetOutput(), outputFilename);
  }
  else
  {
    ITKHelpers::WriteImage(flipFilter->GetOutput(), outputFilename);
  }

  return EXIT_SUCCESS;
}
