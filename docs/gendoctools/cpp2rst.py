"""Tools used during configuration and build process of
Siconos documentation.

This modules contains functions dedicated to the generation
of rst files from xml output generated by doxygen,
from c++ inline documentation.

Process :
hpp (C++) --> doxygen --> xml

xml --> this --> rst files in a 'breathe' complient format.


Siconos is a program dedicated to modeling, simulation and control
 of non smooth dynamical systems.

 Copyright 2020 INRIA.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
"""

import os
import textwrap
import re
import buildtools as bt
from pathlib import Path
from gendoctools import common

try:
    import lxml.etree as ET
except ImportError:
    import xml.etree.ElementTree as ET


def create_breathe_files(headers, srcdir, component_name,
                         sphinx_directory, doxygen_config_filename):
    """Create rst files for sphinx from xml (doxygen) outputs generated
       from headers.

    Parameters
    ----------

    headers : list (cmake like)
         headers files to parse
    srcdir : string
        absolute path to c/c++ sources (CMAKE_SOURCE_DIR)
    component_name : string
         component (numerics, kernel, ...) of interest
    sphinx_directory : string
        root directory for sphinx rst files
    doxygen_config_filename : string
         name (full path) of the doxygen configuration file

    Notes:
    * for each header, rst files (class, struct, file and source codes)
      will be generated
    * three other 'main' rst files will be produced :
       * breathe_api.rst, with the toctree for all classes and structs
       * files_list.rst, with the toctree for all files documentation
       * sources_list.rst with the toctree for all program listings

    This function is supposed to be called by a target created with cmake
    (make <component>-xml2rst)

    """

    # Get all headers for the current component, as a list.
    headers = bt.parse_cmake_list(headers)
    rst_files = []
    # Parse doxygen config (specific to the current component)
    doxyconf = common.parse_doxygen_config(doxygen_config_filename)
    xmlconf = {}
    # Output path for cpp api documentation
    sphinx_directory = Path(sphinx_directory, 'reference',
                            'cpp', component_name)
    if not sphinx_directory.exists():
        os.makedirs(sphinx_directory)

    # Check if doxygen doc is case-sensitive
    xmlconf['CASE_SENSE_NAMES'] = doxyconf['CASE_SENSE_NAMES'].find('YES') > -1
    # Get xml files path
    xmlconf['XML_OUTPUT'] = Path(doxyconf['OUTPUT_DIRECTORY'].lstrip(),
                                 doxyconf['XML_OUTPUT'].lstrip())
    all_index = {}
    # -- Create rst for classes, structs and files found in xml directory --
    for hfile in headers:
        xml2rst(Path(hfile), srcdir, component_name, sphinx_directory,
                xmlconf, all_index)

    # -- Create rst files to collect list of classes and files
    # (i.e. files created above) --
    class_and_struct_files = [f for f in sphinx_directory.glob('class*.rst')]
    class_and_struct_files += [f for f in sphinx_directory.glob('struct*.rst')]
    class_and_struct_files.sort()
    pgm_files = [f for f in sphinx_directory.glob('pgm_*.rst')]
    pgm_files.sort()
    rst_files = [f for f in sphinx_directory.glob('file_*.rst')]
    rst_files.sort()
    all_files = class_and_struct_files + rst_files
    all_files.sort()
    # -- Name of the main rst files for the current component --
    # usually : docs/sphinx/reference/cpp/component_name/autodoc_all.rst
    outputname = Path(sphinx_directory, 'autodoc_all.rst')
    autodoc_collect(outputname, all_files, all_index, component_name)
    # Classes and structs
    outputname = Path(sphinx_directory, 'autodoc_classes.rst')
    autodoc_collect(outputname, class_and_struct_files,
                    all_index, component_name, subtitle='Classes and structs')
    # Files doc
    outputname = Path(sphinx_directory, 'autodoc_files.rst')
    autodoc_collect(outputname, rst_files, all_index, component_name,
                    subtitle='Files documentation')
    # Programs listings
    autodoc_collect_pgm(pgm_files, component_name, sphinx_directory)


def xml2rst(headername, srcdir, component_name, sphinx_directory,
            doxyconf, all_index):
    """Generate rst file(s) from xml (doxygen outputs) for given C/C++ header

    Parameters
    ----------
    headername : Path()
        name of the header (full path)
    srcdir : string
        absolute path to c/c++ sources (CMAKE_SOURCE_DIR)
    component_name : string
         component (numerics, kernel, ...) of interest
         (i.e. breathe project)
    sphinx_directory : string
        directory where rst files will be written
    doxyconf : dict
        dict describing xml/doxy conf.
    """
    case_sense_names = doxyconf['CASE_SENSE_NAMES']
    xml_path = doxyconf['XML_OUTPUT']
    # First get list of xml files generated from current header by doxygen
    xml_files = common.get_xml_files(headername, xml_path, case_sense_names)
    # Then, for each xml, write sphinx header.
    # 3 cases : class, struct or file.
    for f in xml_files:
        common.filter_dot_in_xml_formulas(f)
        path = os.path.join(xml_path, f)
        root = ET.parse(path).getroot()
        f = f.as_posix()
        compounds = root.findall('compounddef')
        refname = sphinxref4headername(headername.as_posix(), srcdir)
        name, kind, descr = common.get_xml_compound_infos(compounds[0])
        if f.find('class') > -1 or f.find('struct') > -1:
            assert len(compounds) == 1
            all_index[name] = descr
            assert kind in ('struct', 'class')
            label = '.. _' + kind + '_' + name + ':\n\n'
            title = kind.title() + ' ' + name
            lenname = len(title)
            title = label + title + '\n' + lenname * '-' + '\n\n'
            pgm = 'Defined in :ref:`pgm' + refname + '`' + '\n\n'
            gen = title + pgm
            gen += '.. doxygen' + kind + ':: ' + name + '\n'
            gen += '     :project: ' + component_name + '\n'
            outputname = kind + headername.stem
            outputname = Path(sphinx_directory, outputname + '.rst')

        elif f.find('_8h') > -1:
            label = '.. _file' + refname + ':\n\n'
            shortname = headername.as_posix().split(srcdir)[-1]
            shortname = shortname.replace('/./', '/')
            if shortname[0] == '/':
                shortname = shortname[1:]
            title = 'File ' + shortname
            lenname = len(title)
            title = label + title + '\n' + lenname * '-' + '\n\n'
            pgm = ':ref:`Go to the source code of this file <pgm'
            pgm += refname + '>`' + '\n\n'
            gen = title + pgm
            # sphinx_root = Path(sphinx_directory, '../../')
            #relpath = os.path.relpath(srcdir, sphinx_root)
            #fname = os.path.join(relpath, shortname[1:])
            gen += '.. doxygenfile:: ' + shortname + '\n'
            gen += '     :project: ' + component_name + '\n'
            outputname = 'file_' + headername.name.replace('.', '_')
            outputname = Path(sphinx_directory, outputname + '.rst')
            fname = shortname.split('/')[-1]
            all_index[fname] = descr

        else:  # namespaces files.
            # Nothing to be done, breathe deal with those
            # directly from _8h file.
            continue

        with open(outputname, 'wt') as out:
            out.write(gen)
            out.write('\n')

    create_rst_for_program(headername.as_posix(), srcdir,
                           sphinx_directory, True)


def create_rst_for_program(headername, srcdir, sphinx_directory,
                           filterdox=False):
    """Build rst file from header (c++), for a 'pgm' target in sphinx.


    Parameters
    ----------
    headername : string
        name of the header (full path)
    srcdir : string
        absolute path to c/c++ sources (CMAKE_SOURCE_DIR)
    sphinx_directory : Path()
        directory where rst files will be written
    filterdox : boolean
        true to remove doxygen comments from program listings in sphinx
    """
    shortname = headername.split(srcdir)[-1][1:]
    shortname = shortname.replace(r'./', '')
    outputname = 'pgm_' + shortname.replace('/', '_').replace('.', '_')
    outputname = Path(sphinx_directory, outputname + '.rst')
    refname = sphinxref4headername(headername, srcdir)
    title = 'Program listing for file ' + shortname
    label = '.. _pgm' + refname + ':\n\n'
    lenname = len(title)
    title = label + title + '\n' + lenname * '=' + '\n\n'
    doc = '* Return to documentation for :ref:`this file<file'
    doc += refname + '>`\n\n'
    gen = title + doc
    if filterdox:
        d = filter_comments(headername)
    else:
        with open(headername, 'r') as f:
            d = f.read()
    gen += '.. code-block:: c++\n'
    gen += '    :linenos:\n\n'
    text = textwrap.indent(d, 4 * ' ')
    gen += text

    with open(outputname, 'wt') as out:
        out.write(gen)


def autodoc_collect(outputname, files_list, all_index, component_name,
                    subtitle=None):
    """Create a rst file, autodoc-like, used as table of contents
    either for classes, structs or files (depends on outputname and subtitle)

    Parameters
    ----------
    files_list : list of rst files to take into account
    all_index : dict
        mapping between class names and descriptions (output from xml2rst)
    component_name : string
        current component name
    subtitle : string, optional
        text header for rst file
    """
    basename = '/reference/cpp/' + component_name + '/'
    if subtitle:
        subtitle += '\n' + len(subtitle) * '-' + '\n\n'
    else:
        subtitle = ''
    with open(outputname, 'wt') as out:
        # For each file in the list,
        # create a breathe entry in outputname
        out.write(subtitle)
        for f in files_list:
            name = f.stem
            if 'class' in name:
                shorttitle = name.split('class')[-1]
                text = '* :class:`' + shorttitle + '` : '
            elif 'struct' in name:
                shorttitle = name.split('struct')[-1]
                text = '* :class:`' + shorttitle + '` : '
            elif name.find('file_') > -1:
                shorttitle = name.split('file_')[-1].replace('_', '.')
                name = basename + name
                text = '* :doc:`' + shorttitle + '<' + name + '>` : '
            try:
                text += all_index[shorttitle] + '\n'
            except:
                text += '\n'
            out.write(text)
        out.write('\n\n')


def autodoc_collect_pgm(pgm_files, component_name, sphinx_directory):
    """Create a rst file that list all headers of the current component
    --> table of contents 'files documentation' in html outputs.
    Parameters
    ----------
    pgm_files : list of rst files to take into account
    component_name : string
        current component name
    sphinx_directory : Path
        sphinx root dir (binary)
    """
    outputname = Path(sphinx_directory, 'autodoc_pgm.rst')
    basename = '/reference/cpp/' + component_name + '/'
    sphinx_directory = sphinx_directory.as_posix()
    label = '.. _' + component_name + '_pgm_listings:\n\n'
    title = component_name.title() + ' programs listings\n'
    title += len(title) * '-' + '\n\n'
    title = label + title
    with open(outputname, 'wt') as out:
        out.write(title)
        out.write('.. toctree::\n    :maxdepth: 2\n\n')
        for f in pgm_files:
            name = f.stem
            # Transforms rst file name into header name ...
            splits = name.split('_')
            shorttitle = '/'.join(splits[2:-1]) + '.' + splits[-1]
            name = basename + name
            gen = textwrap.indent(shorttitle + '<' + name + '>\n', 4 * ' ')
            out.write(gen)


def sphinxref4headername(headername, srcdir):
    """Returns the reference to set in sphinx for a given file name

    Parameters
    ----------
    headername : string
        name of the input file (full path)
    srcdir : part of the name to remove from ref (usually CMAKE_SOURCE_DIR)

    """
    if srcdir:
        refname = headername.split(srcdir)[-1]
    else:
        refname = headername
    refname = refname.replace('/', '_')
    return refname


def filter_comments(headername):
    """Returns list of lines from a
    source file, without doxygen comments.
    """
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " "  # note: a space and not an empty string
        return s

    with open(headername, 'r') as f:
        text = f.read()
        pattern = re.compile(
            r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
            re.DOTALL | re.MULTILINE
        )
        return re.sub(pattern, replacer, text)


def build_cpp_api_main(outputdir, rst_header, components):
    """Parse existing rst files (one for each class,
    + those for functions) generated for C++ API
    and collect them into  cpp_api.rst
    in sphinx/reference directory.

    Parameters
    ----------
    outputdir : Path()
         sphinx directory which contains rst files
         generated for the api (e.g. by doxy2swig)
    rst_header : string
         text to put on top of the python_api file.
    """

    mainrst_filename = Path(outputdir, 'cpp_api.rst')
    # list documented (cpp) packages
    docpp_dir = Path(outputdir, 'cpp')
    packages = [f for f in docpp_dir.glob('*')]
    packages = [p.name for p in packages if os.listdir(p)]
    # trick to print components in the expected order.
    packages = [p for p in components if p in packages]
    indent = 4 * ' '
    class_diag = 'Class diagrams (UML view)'
    class_diag += '\n' + len(class_diag) * '=' + '\n\n'
    class_diag += ':doc:`/reference/class_diagrams`\n\n'

    with open(mainrst_filename, 'w') as f:
        label = '.. _siconos_cpp_reference:\n\n\n'
        title = 'Siconos C/C++ API reference'
        title += '\n' + len(title) * '#' + '\n\n'
        title += 'This is the documentation of C/C++ interface to Siconos.\n\n\n'
        f.write(label)
        f.write(title)
        f.write(rst_header)
        tab_directive = '.. csv-table::\n'
        tab_directive += textwrap.indent(':widths: 60 40\n\n', indent)
        column_titles = '**Classes and structs**, **Files**\n'
        tab_directive += textwrap.indent(column_titles, indent)

        f.write(class_diag)
        for p in packages:
            title = p.title() + ' component\n'
            title += len(title) * '=' + '\n\n'
            ppath = 'cpp/' + p
            f.write(title)
            pgm_listings = 'Check :ref:`' + p + '_pgm_listings`'
            pgm_listings += ' for a complete list of headers for this component.'
            f.write(pgm_listings + '\n\n')
            #f.write(tab_directive)
            directive = '.. include:: ' + ppath + '/autodoc_classes.rst'
            directive += '\n'#','
            directive += '.. include:: ' + ppath + '/autodoc_files.rst\n'
            indent = ''
            f.write(textwrap.indent(directive, indent))
            f.write('\n')
