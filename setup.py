# setup.py

from setuptools import setup, find_packages

setup(
    name='PyPHX',
    version='0.1',
    packages=find_packages(),
    install_requires=[
        'numpy>=1.18.0',  # Specify numpy as a required dependency
        'matplotlib>=3.1.0'
        # list your module's dependencies here
    ],
    entry_points={
        'console_scripts': [
            # add command line scripts here
        ],
    },
    author='David Stoker',
    author_email='david.stoker@sri.com',
    description='PyPHX is a python wrapper for a custom PyPHX based on the active silicon PyPHX library',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    url='https://github.com/brillouinzone/PyPHX',  # replace with your module's URL
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
)