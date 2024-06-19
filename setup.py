# setup.py

from setuptools import setup, find_packages

setup(
    name='PyPHX',
    version='0.1',
    packages=find_packages(),
    install_requires=[
        # list your module's dependencies here
    ],
    entry_points={
        'console_scripts': [
            # add command line scripts here
        ],
    },
    author='David Stoker',
    author_email='david.stoker@sri.com',
    description='PyPHX is a python wrapper for a custom phx_api based on the active silicon phx_api library',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    url='https://github.com/brillouinzone/PyPHX',  # replace with your module's URL
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
)