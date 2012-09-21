#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"

//typedef itk::VectorImage<float, 2>  ImageType;
typedef itk::VectorImage<unsigned char, 2>  ImageType;
typedef itk::Image<unsigned char, 2>  MaskType;

// This code replaces every pixel in 'imageFilename' corresponding to a non-zero pixel in 'maskFilename' with 'value'
int main(int argc, char *argv[])
{
  std::cout << "Warning: This uses a simple Image<uchar> for the mask type, not the powerful Mask class in the Mask repository." << std::endl;

  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename maskFilename outputFilename value" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string maskFilename = argv[2];
  std::string outputFilename = argv[3];
  std::string strValue = argv[4];

  std::stringstream ss;
  ss << strValue;
  float value;
  ss >> value;

  // Output arguments
  std::cout << "inputFilename: " << inputFilename << std::endl;
  std::cout << "maskFilename: " << maskFilename << std::endl;
  std::cout << "outputFilename: " << outputFilename << std::endl;
  std::cout << "value: " << value << std::endl;

  // Read image and mask
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer imageReader = ReaderType::New();
  imageReader->SetFileName(inputFilename.c_str());
  imageReader->Update();

  typedef itk::ImageFileReader<MaskType> MaskReaderType;
  MaskReaderType::Pointer maskReader = MaskReaderType::New();
  maskReader->SetFileName(maskFilename.c_str());
  maskReader->Update();

  // Create output image
  ImageType::Pointer output = ImageType::New();
  output->SetRegions(imageReader->GetOutput()->GetLargestPossibleRegion());
  output->SetNumberOfComponentsPerPixel(imageReader->GetOutput()->GetNumberOfComponentsPerPixel());
  output->Allocate();

  itk::ImageRegionConstIterator<ImageType> imageIterator(imageReader->GetOutput(), imageReader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<MaskType> maskIterator(maskReader->GetOutput(), maskReader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> outputIterator(output, output->GetLargestPossibleRegion());

  typedef itk::VariableLengthVector<double> VariableVectorType;

  while(!imageIterator.IsAtEnd())
    {
    //if(!maskIterator.Get()) // The pixel is unknown (here we are defining a "known" pixel as white/nonzero)
    if(maskIterator.Get())
      {
      // Set the output image to a pixel with every component set to 'value'
      VariableVectorType pixel;
      pixel.SetSize(imageReader->GetOutput()->GetNumberOfComponentsPerPixel());
      for(unsigned int i = 0; i < pixel.GetSize(); i++)
        {
        pixel[i] = value;
        }
      outputIterator.Set(pixel);
      }
    else // This pixel is known in the mask
      {
      // Copy the input to the output
      outputIterator.Set(imageIterator.Get());
      }

    ++imageIterator;
    ++maskIterator;
    ++outputIterator;
    }

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
