from distutils.core import setup, Extension

setup(name="vbtspa",
      version="0.0.1", 
      description="VBTS Power Amplifier Controller",
      author="Kurtis Heimerl",
      author_email="kheimerl@cs.berkeley.edu",
      url="http://cs.berkeley.edu/~kheimerl",
      license='bsd',
      scripts=["scripts/BM_Get_Condition.py",
               ],
      )
