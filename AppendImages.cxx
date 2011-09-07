#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

typedef itk::VectorImage<float, 2>  ImageType;

// This program takes two images (N channels and M channels) and produces an N+M channel image as output.
// The N channels of image 1 are the first channels of the output, followed by the M channels of image2.
int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: image1Filename image2Filename ... imageNFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::vector<std::string> fileNames;
  for(unsigned int i = 1; i < static_cast<unsigned int>(argc); ++i)
    {
    fileNames.push_back(argv[i]);
    //std::cout << "Adding " << argv[i] << std::endl;
    }
    
  std::string outputFileName = fileNames[fileNames.size() - 1];
  fileNames.erase(fileNames.end());
  /*
  for(unsigned int i = 0; i < fileNames.size(); ++i)
    {
    std::cout << "Remain " << fileNames[i] << std::endl;
    }
  */
  
  
  typedef itk::ImageFileReader<ImageType> ReaderType;
  std::vector<ReaderType::Pointer> readers;
  unsigned int totalComponents = 0;
  for(unsigned int i = 0; i < fileNames.size(); ++i)
    {
    // Read images
    ReaderType::Pointer imageReader = ReaderType::New();
    imageReader->SetFileName(fileNames[i].c_str());
    imageReader->Update();
    readers.push_back(imageReader);
    totalComponents += imageReader->GetOutput()->GetNumberOfComponentsPerPixel();
    /*
    if(image1Reader->GetOutput()->GetLargestPossibleRegion() != image2Reader->GetOutput()->GetLargestPossibleRegion())
      {
      std::cerr << "Images must be the same size!" << std::endl;
      std::cerr << "Image1 is " << image1Reader->GetOutput()->GetLargestPossibleRegion() << std::endl;
      std::cerr << "Image2 is " << image2Reader->GetOutput()->GetLargestPossibleRegion() << std::endl;
      exit(-1);
      }

    std::cout << "Image1 has " << image1Reader->GetOutput()->GetNumberOfComponentsPerPixel() << " components." << std::endl;
    std::cout << "Image2 has " << image2Reader->GetOutput()->GetNumberOfComponentsPerPixel() << " components." << std::endl;
    */
    }

  // Create output image
  itk::ImageRegion<2> region = readers[0]->GetOutput()->GetLargestPossibleRegion();

  std::cout << "Output image has " << totalComponents << " components." << std::endl;

  ImageType::Pointer output = ImageType::New();
  output->SetNumberOfComponentsPerPixel(totalComponents);
  output->SetRegions(region);
  output->Allocate();

  // Combine images
  unsigned int currentComponent = 0;
  for(unsigned int readerId = 0; readerId < readers.size(); ++readerId)
    {
    itk::ImageRegionConstIterator<ImageType> imageIterator(readers[readerId]->GetOutput(), region);
    itk::ImageRegionIterator<ImageType> outputIterator(output, region);

    while(!imageIterator.IsAtEnd())
      {
      ImageType::PixelType pixel = outputIterator.Get();
      //std::cout << "Pixel has " << pixel.GetNumberOfElements() << " elements" << std::endl;
      
      for(unsigned int component = 0; component < readers[readerId]->GetOutput()->GetNumberOfComponentsPerPixel(); component++)
	{
	pixel[currentComponent + component] = imageIterator.Get()[component];
	}
    
      outputIterator.Set(pixel);

      ++imageIterator;
      ++outputIterator;
      }
      
    currentComponent += readers[readerId]->GetOutput()->GetNumberOfComponentsPerPixel();
    std::cout << "currentComponent is now: " << currentComponent << std::endl;
    }

  std::cout << "Writing " << outputFileName << std::endl;

  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(output);
  writer->Update();
  
  return EXIT_SUCCESS;
}
