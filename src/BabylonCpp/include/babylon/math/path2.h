#ifndef BABYLON_MATH_PATH2_H
#define BABYLON_MATH_PATH2_H

#include <babylon/babylon_global.h>

namespace BABYLON {

/**
 * @brief Represents a Path2.
 */
class BABYLON_SHARED_EXPORT Path2 {

public:
  /**
   * @brief Creates a Path2 object from the starting 2D coordinates x and y.
   */
  Path2(float x, float y);
  Path2(const Path2& otherPath);
  Path2(Path2&& otherPath);
  Path2& operator=(const Path2& otherPath);
  Path2& operator=(Path2&& otherPath);
  ~Path2();
  Path2 copy() const;
  std::unique_ptr<Path2> clone() const;
  friend std::ostream& operator<<(std::ostream& os, const Path2& path);

  /**
   * @brief Adds a new segment until the passed coordinates (x, y) to the
   * current Path2.
   * @returns The updated Path2.
   */
  Path2& addLineTo(float x, float y);

  /**
   * @brief Adds _numberOfSegments_ segments according to the arc definition
   * (middle point coordinates, end point coordinates, the arc start point being
   * the current Path2 last point) to the current Path2.
   * @returns The updated Path2.
   */
  Path2& addArcTo(float midX, float midY, float endX, float endY,
                  unsigned int numberOfSegments = 36);

  /**
   * @brief Closes the Path2.
   * @returns The Path2.
   */
  Path2& close();

  /**
   * @brief Returns the Path2 total length (float).
   */
  float length() const;

  /**
   * @brief Returns the Path2 internal array of points.
   */
  std::vector<Vector2>& getPoints();
  const std::vector<Vector2>& getPoints() const;

  /**
   * @brief Returns a new Vector2 located at a percentage of the Path2 total
   * length on this path.
   */
  Vector2 getPointAtLengthPosition(float normalizedLengthPosition) const;

  /** Statics **/

  /**
   * @brief Returns a new Path2 starting at the coordinates (x, y).
   */
  static Path2 StartingAt(float x, float y);

public:
  bool closed;

private:
  std::vector<Vector2> _points;
  float _length;

}; // end of class Path2

std::ostream& operator<<(std::ostream& os, const Path2& path);

} // end of namespace BABYLON

#endif // end of BABYLON_MATH_PATH2_H
