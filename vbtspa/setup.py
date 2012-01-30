from distutils.core import setup, Extension

setup(name="vbtspa",
      version="0.0.2", 
      description="VBTS Power Amplifier Controller",
      author="Kurtis Heimerl",
      author_email="kheimerl@cs.berkeley.edu",
      url="http://cs.berkeley.edu/~kheimerl",
      license='bsd',
      scripts=[
        "scripts/VBTS_PA_Controller.py",
        "scripts/VBTS_PA_On.py",
        "scripts/VBTS_PA_Off.py",
        ],
      )
