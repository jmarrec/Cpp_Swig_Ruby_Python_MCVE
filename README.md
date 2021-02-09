# C++ SWIG Wrappers: passing info from Ruby bindings to Python bindings via Pycall

This is a weird application, but we have a large C++ project with bindings to ruby and python (and C#).

A lot of utilities have been built using the ruby bindings. Today, we are trying to add some functionality in Python, without having to duplicate and rewrite all facilities.

The gist is this: we have a ruby gem that will do a lot of work to run simulations.
You can write multiple ruby measures and pass them in a configuration file to the ruby gem, and it will evaluate them by running their `run` method. See below for more context as needed

We are trying to use [mrkn/PyCall](https://github.com/mrkn/pycall.rb) from ruby. But hitting problems when passing SWIG'ed objects to/from ruby to python


### Getting setup for the MCVE

This assumes you have SWIG, Python and Ruby available in your path (Note: Tested with SWIG 4.0.2, Python 3.9 and Ruby 2.5.5).

In the root of the directory:

```
mkdir build
cd build
cmake ../
make
```

Then try to run `python/test_python_only.py` and `ruby/test_ruby_only.rb`. Both should work.

Install the pycall ruby gem: `gem install pycall -v '1.2.1'`.

Then try `ruby/test_pycall.rb` and see it fail...

All of these tests are also registered as `CTest`, so you can run the entire test suite via `make test` or `ctest` (`ctest -C Release` on Windows)


### TL;DR: More context

To simplify a Measure is defined as the following C++ interface

```c++
// C++
namespace openstudio {
    class Measure {
      public:
        virtual ~ModelMeasure();

        // Arguments are definitely not POD types
        virtual bool run(openstudio::model::Model& model, OSRunner& runner, const std::map<std::string, OSArgument>& user_arguments) const;

    }
}
```

Each ruby measure is therefore as this, inheriting from the SWIG'ed ruby type:

```ruby
# Ruby
class MyRubyMeasure < OpenStudio::Measure

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # DO STUFF

    return true
  end
end
```

It's the "run" method that is being called from the ruby gem.

What we want to do is to keep the ruby gem as is, but allow using measures written in python.

```python
# Python
class MyPythonMeasure(openstudio.measure):

    def run(self,
            model: openstudio.model.Model,
            runner: openstudio.measure.OSRunner,
            user_arguments: openstudio.measure.OSArgumentMap):

        # DO STUFF

        return True
```
