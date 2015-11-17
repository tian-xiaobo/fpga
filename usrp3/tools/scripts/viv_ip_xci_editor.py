#!/usr/bin/python

import argparse
import os, sys
import re

# Parse command line options
def get_options():
    parser = argparse.ArgumentParser(description='Utility script to query and modify a Xilinx IP XCI file')
    parser.add_argument('action', type=str, default=None, help='Action to perform')
    parser.add_argument('xci_filepath', type=str, default=None, help='Name for the IP core')
    parser.add_argument('--target', type=str, default=None, help='Input value for target. Must be of the form <arch>/<device>/<package>/<speedgrade>')
    parser.add_argument("--output_dir", type=str, default='.', help="Build directory for IP")
    args = parser.parse_args()
    if not args.action:
        print 'ERROR: Please specify an action to perform\n'
        parser.print_help()
        sys.exit(1)
    if not args.xci_filepath:
        print 'ERROR: Please specify the location for the XCI file to operate on\n'
        parser.print_help()
        sys.exit(1)
    if (not os.path.isfile(args.xci_filepath)):
        print('ERROR: XCI File ' + args.xci_filepath + ' could not be accessed or is not a file.\n')
        parser.print_help()
        sys.exit(1)
    return args

def get_match_str(item):
    return '(.*\<spirit:configurableElementValue spirit:referenceId=\".*\.' + item + '\"\>)(.+)(\</spirit:configurableElementValue\>)'

def main():
    args = get_options();
    
    # Read XCI File
    with open(args.xci_filepath) as in_file:
        xci_lines = in_file.readlines()

    if args.action.startswith('read_'):
        # Extract info from XCI File
        xci_info = dict()
        for line in xci_lines:
            m = re.search(get_match_str('(ARCHITECTURE|DEVICE|PACKAGE|SPEEDGRADE)'), line)
            if m is not None:
                 xci_info[m.group(2)] = m.group(3)
        if args.action == 'read_target':
            print xci_info['ARCHITECTURE'] + '/' + xci_info['DEVICE'] + '/' + xci_info['PACKAGE'] + '/' + xci_info['SPEEDGRADE'];
        if args.action == 'read_arch':
            print xci_info['ARCHITECTURE'];
        if args.action == 'read_partid':
            print xci_info['DEVICE'] + '/' + xci_info['PACKAGE'] + '/' + xci_info['SPEEDGRADE'];
        if args.action == 'read_part':
            print xci_info['DEVICE'] + xci_info['PACKAGE'] + xci_info['SPEEDGRADE'];
    elif args.action == 'retarget':
        # Write a new XCI file with modified target info
        if (not os.path.isdir(args.output_dir)):
            print('ERROR: IP Build directory ' + args.output_dir + ' could not be accessed or is not a directory.')
            sys.exit(1)
        if not args.target:
            print('ERROR: No target specified.')
            sys.exit(1)
        target_tok = args.target.split('/')
        if len(target_tok) != 4:
            print('ERROR: Invalid target format. Must be <arch>/<device>/<package>/<speedgrade>')
            sys.exit(1)

        replace_dict = {'ARCHITECTURE': target_tok[0], 'DEVICE': target_tok[1], 'PACKAGE': target_tok[2], 'SPEEDGRADE': target_tok[3], \
                        'C_XDEVICEFAMILY': target_tok[0], 'C_FAMILY': target_tok[0], 'C_XDEVICE': target_tok[1]}
        out_xci_filename = os.path.join(os.path.abspath(args.output_dir), os.path.basename(args.xci_filepath)) 
        
        with open(out_xci_filename, 'w') as out_file:
            for r_line in xci_lines:
                w_line = r_line
                m = re.search(get_match_str('(' + '|'.join(replace_dict.keys()) + ')'), r_line)
                if m is not None:
                    w_line = m.group(1) + replace_dict[m.group(2)] + m.group(4) +'\n'
                out_file.write(w_line)


if __name__ == '__main__':
    main()
