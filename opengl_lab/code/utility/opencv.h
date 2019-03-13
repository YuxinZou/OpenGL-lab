// disable warnings caused by opencv
#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4265 ) // warning C4265: "...": Die Klasse besitzt virtuelle Funktionen, aber der Destruktor ist nicht virtuell
#pragma warning( disable : 4548 ) // warning C4548: Ausdruck vor dem Komma hat keine Auswirkung; es wurde ein Ausdruck mit Nebeneffekt erwartet
#pragma warning( disable : 4190 ) // warning C4190: "..." hat C-Bindung angegeben, aber gibt UDT "..." zurück, was mit C inkompatibel ist
#endif

#if OPENCV_VERSION == 3
#include <opencv2/core.hpp>
#include <opencv2/flann.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#else
#include <opencv2/core/core.hpp>
#include <opencv2/flann/flann.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

// re-enable warnings
#ifdef WIN32
#pragma warning( pop )
#endif