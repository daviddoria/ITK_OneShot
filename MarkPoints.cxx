#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

typedef itk::RGBPixel<unsigned char> RGBPixelType;
typedef itk::Image<RGBPixelType, 2>  ImageType;

void MarkPatch(ImageType::Pointer image, itk::Index<2> center, unsigned int radius);

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Required: image points" << std::endl;
    return -1;
    }

  typedef itk::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  // Read points
  std::ifstream pointsstream(argv[2]);

  if(pointsstream == NULL)
    {
    std::cout << "Cannot open file " << argv[2] << std::endl;
    exit(-1);
    }

  // Read the point from the first image
  std::string line;

  while(getline(pointsstream, line))
  {
    std::stringstream ss(line);
    float x,y;
    ss >> x >> y;
    itk::Index<2> index;
    index[0] = x;
    index[1] = y;

    MarkPatch(reader->GetOutput(), index, 5);

  }

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("markedPoints.png");
  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

void MarkPatch(ImageType::Pointer image, itk::Index<2> center, unsigned int radius)
{
  RGBPixelType red;
  red.SetRed(255);
  red.SetGreen(0);
  red.SetBlue(0);

  for(int i = center[0] - radius; i < center[0] + radius; ++i)
  {
    for(int j = center[1] - radius; j < center[1] + radius; ++j)
    {
    itk::Index<2> index;
    index[0] = i;
    index[1] = j;
    image->SetPixel(index, red);
    }
  }
}
