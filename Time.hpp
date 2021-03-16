#ifndef TIME_HPP
#define TIME_HPP

#include <ctime>

namespace test {

class Time {
  public:

    Time()
      : m_time(std::time(nullptr))
    {
    }

    std::time_t getTime() const {
      return m_time;
    }
    void setTime(const std::time_t& t_time) {
      m_time = t_time;
    };

  private:
    std::time_t m_time;
};

} // namespace test

#endif // TIME_HPP
