#ifndef RAPID_EMITTER_H
#define RAPID_EMITTER_H

#include "rapid_generator/rapid_data_structures.h"

#include <iosfwd>
#include <string>

namespace rapid_emitter
{

struct TrajectorySegment
{
  enum SegmentType { PROCESS, APPROACH, TRAVERSE };
  SegmentType type;
  std::vector<TrajectoryPt> points;
};

/**
 * @brief Writes a RAPID program to 'os' that moves through a sequence of joint positions
 *        and toggles an I/O on and off at specific sequence points.
 * @param os The output ostream object: could be a file handle or string stream
 * @param approach Trajectory representing phase 1 of the process path - approach to the first segment
 * @param departure The trajectory representing motion away from the last segment
 * @param segments The sequence, in order, of process paths - enumerated by type:process, approach, or traverse.
 *                 This only changes your tool velocities.
 * @param params The set of parameters that indicate velocities for different phase of the project, and other,
 *               possibly optional process parameters.
 * @return True if a file was generated; false if there was some kind of IO error
 */
bool emitRapidFile(std::ostream& os, const std::vector<TrajectoryPt>& approach,
                   const std::vector<TrajectoryPt>& departure, const std::vector<TrajectorySegment>& segments,
                   const ProcessParams& params);

/**
 * @brief Writes a RAPID program to 'os' that merely moves through a sequence of points.
 * @param os  The output stream; will usually be std::ofstream
 * @param points  Sequence of joint positions and durations to move through
 * @param params  These parameters are used to provide default motion speeds if duration is left
 * emtpy
 * @return True is the file was successfully generated
 */
bool emitJointTrajectoryFile(std::ostream& os, const std::vector<TrajectoryPt>& points,
                             const ProcessParams& params);

/** Helper Functions **/

// Writes a joint target with id 'n' to the pre-amble section of a custom module
bool emitJointPosition(std::ostream& os, const TrajectoryPt& pt, size_t n);
// Writes a linear motion to joint target 'n'; the final parameters control special parameters for
// Wolf systems
bool emitGrindMotion(std::ostream& os, const ProcessParams& params, size_t n, bool start = false,
                     bool end = false);
// Writes a free joint move to target 'n'
bool emitFreeMotion(std::ostream& os, const ProcessParams& params, size_t n, double duration,
                    bool stop_at);
// Writes an I/O using the name from 'params'
bool emitSetOutput(std::ostream& os, const ProcessParams& params, size_t value);
// Writes the necessary info for process parameter blocks
bool emitProcessDeclarations(std::ostream& os, const ProcessParams& params, size_t value);
}

#endif
