# Hut

Hut is an early-stage local key-value store designed for high-performance, with the following characteristics:

  - Designed for flash-based storage
  - Low write amplification by separing metadata from data
  - Low metadata storage overhead
  - Workload-agnostic – automatic on-drive optimization of hot data for update or delete-heavy use-cases
  - Fast, zero-copy reads and writes using memory maps

### License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program. If not, see http://www.gnu.org/licenses/. A copy of the GNU Lesser General Public License is in the file COPYING.
