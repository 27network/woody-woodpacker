#!/usr/bin/env python3
"""
ELF LOAD Segment Alignment Checker

This script uses readelf to extract LOAD segment information from an ELF file
and validates that each segment's virtual address and file offset are properly
aligned according to the ELF specification.
"""

import subprocess
import sys
import re
from typing import List, NamedTuple, Optional

class LoadSegment(NamedTuple):
    """Represents a LOAD segment from readelf output"""
    offset: int
    vaddr: int
    paddr: int
    filesz: int
    memsz: int
    flags: str
    align: int

def parse_readelf_output(output: str) -> List[LoadSegment]:
    """Parse readelf -lW output and extract LOAD segments"""
    segments = []
    
    # Look for LOAD segments in the output
    # Format: LOAD  0x000000 0x0000000000000000 0x0000000000000000 0x000747 0x000747 R E 0x1000
    # Note: Flags can contain spaces (like "R E"), so we need to be careful with parsing
    
    for line in output.split('\n'):
        line = line.strip()
        if not line.startswith('LOAD'):
            continue
            
        # Split the line but handle the flags field specially
        parts = line.split()
        if len(parts) < 7:
            continue
            
        try:
            # The alignment is always the last field
            align = int(parts[-1], 16)
            
            # Extract the numeric fields (first 5 after LOAD)
            offset = int(parts[1], 16)
            vaddr = int(parts[2], 16) 
            paddr = int(parts[3], 16)
            filesz = int(parts[4], 16)
            memsz = int(parts[5], 16)
            
            # The flags are everything between memsz and align
            # Join them back with spaces since flags can contain spaces
            flags = ' '.join(parts[6:-1])
            
            segments.append(LoadSegment(offset, vaddr, paddr, filesz, memsz, flags, align))
            
        except (ValueError, IndexError) as e:
            print(f"Warning: Failed to parse line: {line}")
            print(f"Error: {e}")
    
    return segments

def check_alignment(segment: LoadSegment) -> bool:
    """Check if a segment satisfies ELF alignment requirements"""
    if segment.align <= 1:
        # No alignment required
        return True
    
    # Check if p_vaddr ≡ p_offset (mod p_align)
    return (segment.vaddr % segment.align) == (segment.offset % segment.align)

def run_readelf(filepath: str) -> Optional[str]:
    """Run readelf -lW on the given file and return output"""
    try:
        result = subprocess.run(['readelf', '-lW', filepath], 
                              capture_output=True, text=True, check=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        print(f"Error running readelf: {e}")
        print(f"stderr: {e.stderr}")
        return None
    except FileNotFoundError:
        print("Error: readelf not found. Please install binutils.")
        return None

def main():
    if len(sys.argv) != 2:
        print("Usage: python elf_checker.py <elf_file>")
        sys.exit(1)
    
    filepath = sys.argv[1]
    
    # Run readelf
    print(f"Analyzing ELF file: {filepath}")
    output = run_readelf(filepath)
    if output is None:
        sys.exit(1)
    
    # Parse LOAD segments
    segments = parse_readelf_output(output)
    if not segments:
        print("No LOAD segments found in the output")
        sys.exit(1)
    
    print(f"\nFound {len(segments)} LOAD segment(s):")
    print()
    
    all_compliant = True
    
    for i, segment in enumerate(segments, 1):
        print(f"Segment {i}:")
        print(f"  Offset:   0x{segment.offset:06x}")
        print(f"  VAddr:    0x{segment.vaddr:016x}")
        print(f"  FileSz:   0x{segment.filesz:06x}")
        print(f"  MemSz:    0x{segment.memsz:06x}")
        print(f"  Flags:    {segment.flags}")
        print(f"  Align:    0x{segment.align:x}")
        
        # Check alignment
        is_compliant = check_alignment(segment)
        
        if segment.align <= 1:
            print(f"  Status:   ✓ No alignment required")
        else:
            offset_remainder = segment.offset % segment.align
            vaddr_remainder = segment.vaddr % segment.align
            
            print(f"  Offset % Align: 0x{segment.offset:x} % 0x{segment.align:x} = 0x{offset_remainder:x}")
            print(f"  VAddr  % Align: 0x{segment.vaddr:x} % 0x{segment.align:x} = 0x{vaddr_remainder:x}")
            
            if is_compliant:
                print(f"  Status:   ✓ Compliant (remainders match)")
            else:
                print(f"  Status:   ✗ NON-COMPLIANT (remainders don't match)")
                all_compliant = False
        
        print()
    
    # Summary
    if all_compliant:
        print("✓ All LOAD segments are ELF-compliant!")
    else:
        print("✗ Some LOAD segments are NOT ELF-compliant!")
        sys.exit(1)

if __name__ == "__main__":
    main()
